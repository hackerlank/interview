<?php

class MintTask extends MintObjectProxy {

    const STATUS_NEW = 0;
    const STATUS_RUNNING = 1;
    const STATUS_CLOSED = 2;
    const STATUS_CANCELLED = 3;
    const STATUS_HOLD = 4;

    public $pk_field = 'id';
    public $table = 'public.tasks';
    public $sequence = 'public.tasks_seq_id';
    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'updated' => array('type' => 'timestamp'),
        'closed' => array('type' => 'timestamp'),
        'created_by' => array('type' => 'bigint', 'not_null' => true),
        'updated_by' => array('type' => 'bigint'),
        'closed_by' => array('type' => 'bigint'),
        'priority' => array('type' => 'smallint', 'default' => 1),
        'status' => array('type' => 'smallint', 'default' => self::STATUS_NEW),
        'progress' => array('type' => 'smallint', 'default' => 0),
        'title' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'body' => array('type' => 'text', 'not_null' => true, 'not_empty' => true),
        'deadline' => array('type' => 'timestamp'),
    );

}