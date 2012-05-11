<?php

final class MintClientPerson extends MintClientResident {

    static $fields = array(
        'type' => array('type' => 'smallint', 'default' => 1, 'comment' => '0 - undefined, 1 - person, 2 - org, 3 - ip, 4 - non-resident'),
        'birthdate' => array('type' => 'date', 'not_null' => true),
        'passport_serie' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'passport_number' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'passport_mnt_by' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),

    );
}