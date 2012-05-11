<?php

class MintCustomModelType extends MintActiveRecord {

    public $pk_field = 'id';
    public $table = 'public.custom_model_types';
    public $sequence = 'public.custom_model_types_seq_id';
    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'model' => array('type' => 'varchar(64)', 'not_null' => true),
        'name_ru' => array('type' => 'varchar(64)', 'not_null' => true),
        'schema' => array('type' => 'varchar(64)', 'not_null' => true),
        'table' => array('type' => 'varchar(64)', 'not_null' => true),
        'pk_field' => array('type' => 'varchar(64)', 'not_null' => true),
        'pk_type' => array('type' => 'varchar(64)', 'not_null' => true),
        'sequence' => array('type' => 'varchar(128)'),
        'connection_id' => array('type' => 'varchar(32)', 'not_null' => true, 'default' => MINT_DEFAULT_CONNECTION_ID),
    );

    public static function exists($model) {

        $params = array();
        $params[] = array("model = ?", array($model));

        return self::getOne($params) != null;
    }

    public function getCustomModelFields() {

        $params = array();
        $params[] = array('model = ?', array($this->model));

        $result = array();

        foreach(MintCustomModelField::getByParams($params) as $field) {

            if($field->name != $this->pk_field) {
                $result = array_merge($result, $field->toArray());
            }
        }

        return $result;
    }
}