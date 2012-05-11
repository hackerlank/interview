<?php

class MintDBO {

    protected $pdo;
    protected $dsn;

    public $engine;
    public $connection_id;

    public function __construct($db_name, $db_host, $db_user, $db_pass, $db_engine, $db_connection_id) {

        $this->engine = $db_engine;
        $this->connection_id = $db_connection_id;

        $this->dsn = sprintf("%s:dbname=%s;host=%s", $this->engine, $db_name, $db_host);
        $this->pdo = new PDO($this->dsn, $db_user, $db_pass);
        $this->pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        if($this->engine == 'pgsql') {
            $this->pdo->query('SET DATESTYLE = ISO, European');
        }
    }

    public function begin() {

        return $this->pdo->beginTransaction();
    }

    public function commit() {

        return $this->pdo->commit();
    }

    public function rollback() {

        return $this->pdo->rollBack();
    }

    public function query($Q, $params = array()) {

        $params = !empty($params)&&is_array($params)? $params : array();
        $sth = $this->pdo->prepare($Q);

        foreach($params as $k=>$v) {
            $k = is_int($k)? $k+1 : $k;
            switch(true) {
                case is_bool($v):
                    $sth->bindValue($k, $v, PDO::PARAM_BOOL);
                    break;
                case is_integer($v):
                    $sth->bindValue($k, $v, PDO::PARAM_INT);
                    break;
                case is_null($v):
                    $sth->bindValue($k, $v, PDO::PARAM_NULL);
                    break;
                default:
                    $sth->bindValue($k, $v, PDO::PARAM_STR);
                    break;
            }
        }

        $sth->execute();
        return $sth;
    }

    public function getOne($q, $params = array()) {

        $sth = $this->query($q, $params);
        $row = $sth->fetch(PDO::FETCH_NUM);

        if(is_array($row)) {
            return $row[0];
        }

        return null;
    }
    
    public function fetchAll($q, $params = array()) {
        $sth = $this->query($q,$params);
        if($sth) {
            return $sth->fetchAll(PDO::FETCH_ASSOC);
        } else {
            return array();
        }
    }

	public function getRow($q, $params = array(), $fetch = PDO::FETCH_ASSOC) {
		$sth = $this->query($q, $params);
		if($sth) {
		    return $sth->fetch($fetch);
		} else {
		    return array();
		}
	}

    public function getLastInsertId($name = NULL) {

        return $this->pdo->lastInsertId($name);
    }
}