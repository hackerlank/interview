<?php

class MailerLog extends MintActiveRecord {

    public $pk_field = 'id';
    public $table = 'maintain.mailer_log';
    public $sequence = 'maintain.mailer_log_id_seq';

    static $fields = array(

        'created' => array('type' => 'timestamp', 'default' => 'now', 'not_null' => true),
        'to' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'from' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'reply_to' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'subject' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'body' => array('type' => 'text', 'not_null' => true, 'not_empty' => true),
    );
}