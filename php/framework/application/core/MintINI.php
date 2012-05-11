<?php

class MintINI extends Singleton {

    public $data = array();

    public function __construct() {

        $this->data = parse_ini_file(MINT_CONF_PATH);

        if($this->data === false) {
            throw new MintINIException('Failed to load config. MINT_CONF_PATH is: ' . MINT_CONF_PATH);
        }
    }

    public function getAll() {

        return $this->data;
    }

    public function getValue($key) {

        if(!isset($this->data[$key])) {
            throw new MintINIException('Missing key: ' . $key);
        }

        return $this->data[$key];
    }

    public static function create($file, $params) {


    }
}