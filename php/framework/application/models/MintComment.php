<?php

class MintComment extends MintObjectProxy {

    public $pk_field = 'id';
    public $table = 'public.comments';
    public $sequence = 'public.comments_seq_id';
    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'user_id' => array('type' => 'bigint'),
        'manager_id' => array('type' => 'bigint'),
        'parent_id' => array('type' => 'bigint'),
    );
}