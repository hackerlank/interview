<?php

abstract class MintClientResident extends MintClient {

    static $fields = array(
        'is_resident' => array('type' => 'boolean', 'not_null' => true, 'default' => true),
        'country' => array('type' => 'varchar', 'not_null' => true, 'default' => 'Russia'),

    );
}

/*
 *
	    'is_resident' => array('type' => 'boolean', 'default' => 'true'),
	    'is_active' => array('type' => 'boolean', 'default' => 'true'),

	    'type' => array('type' => 'smallint', 'default' => 0, 'comment' => '0 - undefined, 1 - person, 2 - org, 3 - non-resident'),
	    'external_uid' => array('type' => 'bigint', 'default' => NULL, 'comment' => 'идентификатор для связи с внешними системами'),

	    'name_en' => array('type' => 'varchar(256)'),
	    'name_ru' => array('type' => 'varchar(256)'),

	    'birthdate' => array('type' => 'date'),
	    'passport_serie' => array('type' => 'varchar'),
	    'passport_number' => array('type' => 'varchar'),
	    'passport_mnt_by' => array('type' => 'varchar'),

	    'inn' => array('type' => 'varchar(12)'),
	    'bik' => array('type' => 'varchar(11)'),
	    'kpp' => array('type' => 'varchar(9)'),
	    'okpo' => array('type' => 'varchar'),
	    'okved' => array('type' => 'varchar'),
	    'okato' => array('type' => 'varchar'),
	    'rs' => array('type' => 'varchar'),
	    'ks' => array('type' => 'varchar'),
	    'ogrn' => array('type' => 'varchar'),
	    'ogrnip' => array('type' => 'varchar'),
	    'ogrnip_date' => array('type' => 'date'),
	    'bank_name' => array('type' => 'varchar'),
 */