<?php

class MintRegistryEntry extends MintActiveRecord {

    public $pk_field = 'key';
    public $pk_type = 'string';
    public $table = 'maintain.registry';
    public $sequence = 'maintain.registry_id_seq';

    static $fields = array(
        'id' => array('type' => 'integer', 'not_null' => true),
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'value' => array('type' => 'varchar(64)', 'not_null' => true),
    );

    public static function exists($key) {
        
        try {

            return parent::exists($key);

        } catch (PDOException $e) {

            return false;
        }
    }
}