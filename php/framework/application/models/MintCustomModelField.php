<?php

class MintCustomModelField extends MintActiveRecord {

    public $pk_field = 'id';
    public $table = 'public.custom_model_fields';
    public $sequence = 'public.custom_model_fields_seq_id';
    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'model' => array('type' => 'varchar(64)', 'not_null' => true),
        'name' => array('type' => 'varchar(64)', 'not_null' => true),
        'not_null' => array('type' => 'boolean', 'not_null' => true, 'default' => false),
        'default' => array('type' => 'varchar(64)'),
        'type' => array('type' => 'varchar(64)', 'not_null' => true),
    );

    public function toArray() {

        return array($this->name => array(
            'type' => $this->type,
            'default' => $this->default,
            'not_null' => $this->not_null));
    }
}