<?php

class MintMenuItem extends MintActiveRecord {

    public $pk_field = 'id';
    public $table = 'maintain.menu';
    public $sequence = 'maintain.menu_id_seq';

    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'parent_id' => array('type' => 'integer'),
        'title' => array('type' => 'varchar(64)'),
        'acl_id' => array('type' => 'integer', 'not_null' => true, 'not_empty' => true),
    );

}