<?php

class MintConnectionPool extends Singleton {

    private $_pool = array();

    public function __construct() {

        $this->loadConnections();
    }

    private function loadConnections() {

        $ini = MintINI::getInstance();

        $n = 1;

        try {

            // Алгоритм такой: пытаемся получить информацию по n+1 подключению
            // пока MintINI не выкинет эксепшон (это значит что такого ключа в ini-файле нет
            // значит мы загрузили все описанные подключения

            for(;;) {

                $db_name = $ini->getValue("db{$n}_name");
                $db_host = $ini->getValue("db{$n}_host");
                $db_user = $ini->getValue("db{$n}_user");
                $db_pass = $ini->getValue("db{$n}_pass");
                $db_engine = $ini->getValue("db{$n}_engine");
                $db_connection_id = $ini->getValue("db{$n}_connection_id");

                $dbo = new MintDBO($db_name, $db_host, $db_user, $db_pass, $db_engine, $db_connection_id);

                if(isset($this->_pool[$db_connection_id])) {
                    throw new MintException(sprintf('Connection with id %s already exists', $db_connection_id));
                }
                
                $this->_pool[$db_connection_id] = $dbo;

                $n++;
            }

        } catch (MintINIException $e) {

        } catch (MintException $he) {

            throw $he;
        }
    }

    public function getDBO($connection_id = MINT_DEFAULT_CONNECTION_ID) {

        if(!isset($this->_pool[$connection_id])) {
            throw new MintException(sprintf('Connection with id %s does not exist.', $connection_id));
        }

        return $this->_pool[$connection_id];
    }
}