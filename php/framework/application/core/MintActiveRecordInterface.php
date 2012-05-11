<?php

interface MintActiveRecordInterface {
    
    public function getPKValue();
    public function load();
    public function save();
    public function delete();

    public static function getAll($sort = '', $table = null);
    public static function getCount($params = array(), $offset = 0, $limit = -1, $table = null);
    public static function getByParams($params = array(), $offset = 0, $limit = -1, $sort = '', $table = null);
    public static function getOne($params = array(), $offset = 0, $sort = '', $table = null);
    public static function getByName($name, $name_field = 'name');
    public static function getByPK($pk_value);

}