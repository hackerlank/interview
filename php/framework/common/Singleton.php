<?php

abstract class Singleton {

    static $instances = array();
    
    static function getInstance($class_name = null) {

        if(is_null($class_name)) {
            $class_name = get_called_class();
        }

        if(!isset(self::$instances[$class_name])) {
            $instance = new $class_name();
            self::$instances[$class_name] = $instance;
        }

        return self::$instances[$class_name];
    }
}