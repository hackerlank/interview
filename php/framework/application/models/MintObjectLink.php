<?php

class MintObjectLink extends MintActiveRecord {

    public $pk_field = 'id';
    public $table = 'public.object_links';
    public $sequence = 'public.object_links_id_seq';
    static $fields = array(
        'parent_id' => array('type' => 'bigint', 'not_null' => true),
        'child_id' => array('type' => 'bigint', 'not_null' => true),
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
    );

    public static function exists($parent_id, $child_id) {

        $params = array();
        $params[] = array('parent_id = ?', array($parent_id));
        $params[] = array('child_id = ?', array($child_id));

        return self::getCount($params) > 0;
    }
}