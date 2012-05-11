<?php

class MintClient extends MintObjectProxy {

    public $pk_field = 'id';
    public $table = 'public.clients';
    public $sequence = 'public.clients_id_seq';

    static $fields = array(
        'created' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'updated' => array('type' => 'timestamp', 'not_null' => true, 'default' => 'now'),
        'login' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'password' => array('type' => 'varchar', 'not_null' => true, 'not_empty' => true),
        'is_resident' => array('type' => 'boolean', 'not_null' => true, 'default' => true),
        'is_active' => array('type' => 'boolean', 'not_null' => true, 'default' => true),
        'external_uid' => array('type' => 'varchar'),
        'type' => array('type' => 'smallint', 'default' => 0, 'comment' => '0 - undefined, 1 - person, 2 - org, 3 - ip, 4 - non-resident'),
        'name_ru' => array('type' => 'varchar'),
        'name_en' => array('type' => 'varchar'),
        'acl' => array('type' => 'bit(128)', 'not_null' => true),
        'created_by' => array('type' => 'integer', 'not_null' => true),
	);

    public static function getByPK($id) {

        $client_type = MintConnectionPool::getInstance()->getDBO()->qetOne("SELECT type FROM {$this->table} WHERE {$this->pk_field} = ?", array($id));

        if(empty($client_type)) {
            throw new Exception('Failed to load class ' . __CLASS__ . ' with id=' . $id);
        }

        $self = null;

        switch(intval($client_type)) {

            case 1:
                $self = MintClientPerson::getByPK($id);
                break;

            case 2:
                $self = MintClientOrg::getByPK($id);
                break;

            case 3:
                $self = MintClientIp::getByPK($id);
                break;

            case 4:
                $self = MintClientNonResident::getByPK($id);
                break;

            case 0:
            default:
                throw new MintUnexpectedSituation('Attempt to load MintClient with unexpected type. Type is: ' . intval($client_type));
                break;
        }

        return $self;
    }

    public static function getByAuth($login, $password) {

        $params = array();
        $params[] = array('login = ?', array($login));
        $params[] = array('password = ?', array($password));

        return self::getOne($params);
    }

}