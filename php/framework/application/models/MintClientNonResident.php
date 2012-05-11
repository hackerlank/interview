<?php

final class MintClientNonResident extends MintClient {

    static $fields = array(
        'type' => array('type' => 'smallint', 'default' => 4, 'comment' => '0 - undefined, 1 - person, 2 - org, 3 - ip, 4 - non-resident'),
        'is_resident' => array('type' => 'boolean', 'not_null' => true, 'default' => false),
        'address_en' => array('type' => 'varchar', 'not_null' => true),
        'country' => array('type' => 'varchar', 'not_null' => true),
    );
}
