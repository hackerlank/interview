<?php

final class MintEvolution {

    public $transaction = false;
    public $category;
    public $to_connection_id;
    public $mutations = array();
    public $filename = null;

    static function getFromFile($filename) {

        if(!file_exists($filename)) {
            throw new Exception('File does not exist: ' . $filename);
        }

        $self = new self();
        $self->filename = $filename;

        $content = file_get_contents($filename);
        $evolution = new SimpleXMLElement($content);
        $evolution_attrs = $evolution->attributes();

        $self->category = isset($evolution_attrs['category']) ? (string)$evolution_attrs['category'] : null;
        $self->transaction = isset($evolution_attrs['transaction']) ? (bool)((string)$evolution_attrs['transaction']) : false;
        $self->to_connection_id = isset($evolution_attrs['connection_id']) ? (string)$evolution_attrs['connection_id'] : MINT_DEFAULT_CONNECTION_ID;

        foreach($evolution->mutation as $mutation) {

            $mutation_attrs = $mutation->attributes();

            $mutation->addChild('category', $self->category);

            $mutation_obj = new MintMutation($mutation);
            $mutation_obj->run_id = isset($mutation_attrs['run_id']) ? intval($mutation_attrs['run_id']) : null;
            $mutation_obj->to_connection_id = $self->to_connection_id;
            $mutation_obj->category = $self->category;

            $self->mutations[] = $mutation_obj;
        }

        return $self;
    }

    public function evolve() {

        if($this->transaction) {
            MintConnectionPool::getInstance()->getDBO($this->to_connection_id)->begin();
        }

        try {

            usort($this->mutations, function($a, $b) { return $a->run_id < $b->run_id ? -1 : 1; });

            MintLogger::getInstance()->log(sprintf("Evolving evolution: %s", $this->filename));

            foreach($this->mutations as $mutation) {

                MintLogger::getInstance()->log(sprintf("Evolving mutation #%s (%s)", $mutation->run_id, $mutation->comment));

                $mutation->evolve();
            }

            foreach($this->mutations as $mutation) {

                $mutation->save();
            }

            if($this->transaction) {
                MintConnectionPool::getInstance()->getDBO($this->to_connection_id)->commit();
            }

        } catch (Exception $e) {

            if($this->transaction) {
                MintConnectionPool::getInstance()->getDBO($this->to_connection_id)->rollback();
            }

            throw new MintEvolutionException($e->getMessage(), 0, null, $mutation->evolve_sql);
        }
    }

    public function rollback() {

        if($this->transaction) {
            MintConnectionPool::getInstance()->getDBO($this->to_connection_id)->begin();
        }

        try {

            MintLogger::getInstance()->log(sprintf("Cancelling evolution: %s", $this->filename));
            
            usort($this->mutations, function($a, $b) { return $a->run_id < $b->run_id ? 1 : -1; });

            foreach($this->mutations as $mutation) {

                MintLogger::getInstance()->log(sprintf("Cancelling mutation #%s (%s)", $mutation->run_id, $mutation->comment));

                $mutation->rollback();
            }

            if($this->transaction) {
                MintConnectionPool::getInstance()->getDBO($this->to_connection_id)->commit();
            }

        } catch (Exception $e) {

            if($this->transaction) {
                MintConnectionPool::getInstance()->getDBO($this->to_connection_id)->rollback();
            }

            throw new MintEvolutionException($e->getMessage(), 0, null, $mutation->rollback_sql);
        }
    }
}