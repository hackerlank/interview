<?php

class MintEvolutionException extends MintException {

    private $sql;

    public function __construct($message, $code = 0, Exception $previous = null, $sql = null) {

        parent::__construct($message, $code, $previous);

        $this->sql = $sql;
    }

    public function getSQL() {

        return $this->sql;
    }
}