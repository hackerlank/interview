<?php

class MintObject extends MintActiveRecord {

    public $pk_field = 'id';
    public $table = 'public.objects';
    public $sequence = 'public.objects_id_seq';
    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'model' => array('type' => 'varchar(64)', 'not_null' => true),
        'foreign_id' => array('type' => 'varchar(32)', 'not_null' => true),
    );

    public static function getByForeignId($model, $foreign_id) {

        $params = array();
        $params[] = array('model = ?', $model);
        $params[] = array('foreign_id = ?', $foreign_id);

        return self::getOne($params);
    }

    public static function exists($id, $model = null) {

        if(is_null($model)) {

            return parent::exists($id);

        } else {

            return (self::getByForeignId($model, $id) != null);
        }
    }
}