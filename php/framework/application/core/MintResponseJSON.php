<?php

class MintResponseJSON {

    const ROOT_ELEMENT = 'result';

    public $request_time;
    public $request_uri;
    public $data;
    public $errors;
    public $success;

    public function serialize() {

        $array = array(self::ROOT_ELEMENT => array(
            (array)$this
        ));

        return json_encode($array);
    }
}