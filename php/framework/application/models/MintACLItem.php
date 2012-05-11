<?php

class MintACLItem extends MintActiveRecord {

    public $pk_field = 'id';
    public $table = 'maintain.acl';
    public $sequence = 'public.acl_id_seq';

    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'bit_position' => array('type' => 'integer', 'not_null' => true),
        'controller_name' => array('type' => 'varchar(64)', 'not_null' => true, 'not_empty' => true),
        'controller_method' => array('type' => 'varchar(64)', 'not_null' => true, 'not_empty' => true),
	);

    static function getByController($controller_name, $controller_method = MINT_CONTROLLER_DEFAULT_METHOD_NAME) {

        $controller_name = str_replace('controller', '', strtolower($controller_name));

        $params = array();
        $params[] = array('controller_name = ?', array($controller_name));
        $params[] = array('controller_method = ?', array($controller_method));

        return self::getOne($params);
    }
}