<?php

class MintManager extends MintObjectProxy {

    public $pk_field = 'id';
    public $table = 'public.managers';
    public $sequence = 'public.managers_id_seq';

    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'login' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'password' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'name' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'acl' => array('type' => 'bit(128)'),
        'is_active' => array('type' => 'boolean', 'not_null' => true, 'default' => true),
        'is_superuser' => array('type' => 'boolean', 'not_null' => true, 'default' => false),
        'created_by' => array('type' => 'integer'),
        'email' => array('type' => 'varchar'),
    );

    static function getByAuth($login, $password) {

        $params = array();
        $params[] = array('login = ?', array($login));
        $params[] = array('password = ?', array($password));

        return self::getOne($params);
    }
}