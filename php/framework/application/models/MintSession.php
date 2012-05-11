<?php

final class MintSession extends MintActiveRecord {

    public $pk_field = 'id';
    public $table = 'maintain.sessions';
    public $sequence = 'maintain.sessions_id_seq';
    static $fields = array(

        'ip' => array('type' => 'inet', 'not_null' => true, 'not_empty' => true),
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'valid_till' => array('type' => 'timestamp', 'not_null' => true),
        'manager_id' => array('type' => 'integer'),
        'client_id' => array('type' => 'integer'),
        'cookie' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
    );

    public static function getByCookie($cookie) {

        $params = array();
        $params[] = array('cookie = ?', array($cookie));

        return self::getOne($params);
    }
}