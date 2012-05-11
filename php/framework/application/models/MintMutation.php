<?php

class MintMutation extends MintActiveRecord {
    
    public $pk_field = 'id';
    public $table = 'maintain.mutations';
    public $sequence = 'maintain.mutations_id_seq';
    static $fields = array(

        'evolve_success' => array('type' => 'boolean'),
        'rollback_success' => array('type' => 'boolean'),

        'evolve_time' => array('type' => 'timestamp'),
        'rollback_time' => array('type' => 'timestamp'),

        'category' => array('type' => 'varchar'),
        'comment' => array('type' => 'varchar'),

        'evolve_type' => array('type' => 'varchar'),
        'rollback_type' => array('type' => 'varchar'),

        'evolve_entity' => array('type' => 'varchar'),
        'rollback_entity' => array('type' => 'varchar'),

        'evolve_user' => array('type' => 'varchar'),
        'rollback_user' => array('type' => 'varchar'),

        'evolve_msg' => array('type' => 'text'),
        'rollback_msg' => array('type' => 'text'),

        'evolve_sql' => array('type' => 'text', 'not_null' => true, 'not_empty' => true),
        'rollback_sql' => array('type' => 'text', 'not_null' => true, 'not_empty' => true),
    );

    public $can_be_saved = true;
    public $run_id = null;
    public $to_connection_id = null;

    public function __construct($xml) {
	
        if(!($xml instanceof SimpleXMLElement)) {
            throw new Exception('Can\'t construct mutation from xml. Input was: ' . $xml);
        }

        $this->category = trim((string)$xml->category);
        $this->comment = trim((string)$xml->comment);
        $this->evolve_sql = trim((string)$xml->evolve_sql);
        $this->rollback_sql = trim((string)$xml->rollback_sql);
        $this->can_be_saved = ((string)$xml->can_be_saved === '1');

        if(preg_match("/^CREATE(.*)/i", $this->evolve_sql)) {

            $this->evolve_type = 'create';
            $this->rollback_type = 'drop';

        } elseif(preg_match("/^ALTER(.*)/i", $this->evolve_sql)) {

            $this->evolve_type = 'alter';
            $this->rollback_type = 'alter';

        } elseif(preg_match("/^DROP(.*)/i", $this->evolve_sql)) {

            $this->evolve_type = 'drop';
            $this->rollback_type = 'create';
        }

        if(preg_match("/(ADD|ALTER|DROP) COLUMN(.*)/i", $this->evolve_sql)) {

            $this->evolve_entity = 'column';
            $this->rollback_entity = 'column';

        } elseif(preg_match("/^(CREATE|ALTER|DROP) TABLE(.*)/i", $this->evolve_sql)) {

            $this->evolve_entity = 'table';
            $this->rollback_entity = 'table';

        } elseif(preg_match("/^(CREATE|ALTER|DROP) SCHEMA(.*)/i", $this->evolve_sql)) {

            $this->evolve_entity = 'schema';
            $this->rollback_entity = 'schema';

        } elseif(preg_match("/^(CREATE|ALTER|DROP) DATABASE(.*)/i", $this->evolve_sql)) {

            $this->evolve_entity = 'database';
            $this->rollback_entity = 'database';
        }
    }

    public function evolve() {

        if($this->evolve_success) {
            throw new Exception("Can't evolve; already evolved");
        }

        try {

            MintConnectionPool::getInstance()->getDBO($this->to_connection_id)->query($this->evolve_sql);

        } catch (Exception $e) {

            $this->evolve_success = false;
            $this->evolve_msg = $e->getMessage();
            throw $e;
        }

        $this->evolve_success = true;
        $this->evolve_msg = 'ok';
        $this->evolve_time = 'now';

        return true;
    }

    public function rollback() {

        if($this->rollback_success) {
            throw new Exception("Can't rollback; already rolled back");
        }

        try {

            if(strlen($this->rollback_sql) > 0) {
                MintConnectionPool::getInstance()->getDBO($this->to_connection_id)->query($this->rollback_sql);
            }

        } catch (Exception $e) {

            $this->rollback_success = false;
            $this->rollback_msg = $e->getMessage();
            throw $e;
        }

        $this->rollback_success = true;
        $this->rollback_msg = 'ok';

        return true;
    }
}