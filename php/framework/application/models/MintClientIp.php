<?php

final class MintClientIp extends MintClientResident {

    static $fields = array(
        'type' => array('type' => 'smallint', 'default' => 3, 'comment' => '0 - undefined, 1 - person, 2 - org, 3 - ip, 4 - non-resident'),
	    'inn' => array('type' => 'varchar'),
	    'bik' => array('type' => 'varchar'),
	    'kpp' => array('type' => 'varchar'),
	    'okpo' => array('type' => 'varchar'),
	    'okved' => array('type' => 'varchar'),
	    'okato' => array('type' => 'varchar'),
	    'rs' => array('type' => 'varchar'),
	    'ks' => array('type' => 'varchar'),
	    'bank_name' => array('type' => 'varchar'),
	    'ogrnip' => array('type' => 'varchar'),
	    'ogrnip_date' => array('type' => 'date'),
    );
}