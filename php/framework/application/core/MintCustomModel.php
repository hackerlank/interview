<?php

abstract class MintCustomModel extends MintObjectProxy {

    static $reflections = array();

    public $pk_field = null;
    public $pk_type = null;
    public $table = null;
    public $sequence = null;
    public $custom_fields = array();
    public $connection_id = null;

    public static function reflect($model) {

        if(!MintCustomModelType::exists($model)) {
            throw new MintException(sprintf("Model %s does not exist.", $model));
        }

        $model_type = MintCustomModelType::getByName($model, 'model');

        self::$reflections[$model] = array();
        self::$reflections[$model]['pk_field'] = $model_type->pk_field;
        self::$reflections[$model]['pk_type'] = $model_type->pk_type;
        self::$reflections[$model]['schema'] = $model_type->schema;
        self::$reflections[$model]['table'] = $model_type->table;
        self::$reflections[$model]['sequence'] = $model_type->sequence;
        self::$reflections[$model]['connection_id'] = $model_type->connection_id;
        self::$reflections[$model]['fields'] = $model_type->getCustomModelFields();
    }

    public function __construct() {

        $model = get_called_class();
        
        if(!isset(self::$reflections[$model])) {
            self::reflect($model);
        }

        $this->pk_field = self::$reflections[$model]['pk_field'];
        $this->pk_type = self::$reflections[$model]['pk_type'];
        $this->table = !empty(self::$reflections[$model]['schema']) ? 
                sprintf("%s.%s", self::$reflections[$model]['schema'], self::$reflections[$model]['table']) :
                self::$reflections[$model]['table'];
        $this->sequence = self::$reflections[$model]['sequence'];
        $this->custom_fields = self::$reflections[$model]['fields'];
        $this->connection_id = self::$reflections[$model]['connection_id'];

        $this->initFields();
    }

}
