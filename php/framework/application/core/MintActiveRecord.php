<?php

abstract class MintActiveRecord implements MintActiveRecordInterface {

    private $fields = array();

    protected $initialized = false;
    protected $map = array();
    protected $__last_op = NULL;
    protected $__pk_value_old = NULL;

    protected $pk_value = NULL;
    public $pk_field = 'id';
    public $pk_type = 'integer'; // valid php type
    public $table = NULL;
    public $sequence = NULL;
    public $loaded = false;
    public $connection_id = MINT_DEFAULT_CONNECTION_ID;

    

    protected function initFields() {
	
        if ($this->initialized) {
            return;
        }

        // Here goes magic
        // Get the name of called class (php 5.3 only)
        $top_class = get_called_class();

        // Getting fields of each parent class (REFLECTION)
        // and merging them with our fields
        $reflection = new ReflectionClass($top_class);

        do {

            if($reflection->hasProperty('custom_fields')) {

                $this->fields = array_merge($this->fields, $reflection->getProperty('custom_fields')->getValue($this));

            } elseif(in_array('fields', array_keys($reflection->getStaticProperties()))) {

                $this->fields = array_merge($this->fields, $reflection->getStaticPropertyValue('fields'));
            }

        } while(($reflection = $reflection->getParentClass()) !== false && $reflection->name != __CLASS__);

        foreach($this->fields as $field => $params) {
            $this->map[$field] = array();
            $this->map[$field]['type'] = $params['type'];
            $this->map[$field]['value'] = null;
            if(isset($params['default'])) {
                $this->map[$field]['value'] = $params['default'];
            }
        }

        $this->map[$this->pk_field]['type'] = $this->pk_type;

        $this->initialized = true;
    }

    protected function nextId() {

        $dbo = MintConnectionPool::getInstance()->getDBO($this->connection_id);

        if($dbo->engine != 'pgsql') {
            throw new MintException('Only PostgreSQL database supports sequences. You cannot call nextId()');
        }

        $id = $dbo->getOne('SELECT nextval(?)', array($this->sequence));

        return intval($id);
    }

    public function getFields() {

        return $this->fields;
    }

    public function getPKValue() {

        return $this->pk_value;
    }

    public function setPKValue($value) {

        $this->{$this->getPKField()} = $value;
    }

    public function getPKField() {

        return $this->pk_field;
    }

    public function __clone() {

        $this->loaded = false;
        $this->{$this->getPKField()} = null;
    }

    public function __get($name) {

        if(!$this->initialized) {

            $this->initFields();
        }

        if(!in_array($name, array_keys($this->map))) {
            throw new MintException(sprintf("[%s:get] Field does not exist: '$name'", get_called_class()));
        }

        return $this->map[$name]['value'];
    }


    public function __set($name, $value) {

        if(!$this->initialized) {

            $this->initFields();
        }

        if($name == $this->pk_field) {

            $this->__pk_value_old = $this->pk_value;
            $this->pk_value = $value;
            settype($this->pk_value, $this->pk_type);

        } elseif(!in_array($name, array_keys($this->map))) {
            $value = var_export($value, true);
            throw new MintException(sprintf("[%s:set] Field does not exist: '$name'", get_called_class()));
        }

        if(is_null($value) && isset($this->map[$name]['not_null']) && $this->map[$name]['not_null']) {
            throw new Exception("Field cannot be NULL: '$name'");
        }

        if(empty($value) && isset($this->map[$name]['not_empty']) && $this->map[$name]['not_empty']) {
            throw new Exception("Field cannot be empty: '$name'");
        }

        $this->map[$name]['value'] = $value;
    }

    public static function toDBType($type, $value) {

        if (is_null($value)) {
            return $value;
        }

        // @todo: performance?
        $type = preg_replace("/\(.*\)/", "", $type);

        switch ($type) {
            case 'text':
            case 'string':
            case 'varchar':
            case 'interval':
            case 'inet':
            case 'bit':
                return strval($value);
                break;

            case 'serial':
            case 'integer':
            case 'bigint':
            case 'smallint':
            case 'int':
            case 'tinyint':
                return intval($value);
                break;

            case 'numeric':
            case 'float':
            case 'double precision':
            case 'double':
                return floatval($value);
                break;

            case 'boolean':
                if (is_bool($value)) {
                    return $value;
                } elseif (empty($value)) {
                    return false;
                } else {
                    return ($value != 'f');
                }
                break;

            case 'date':
            case 'datetime':
            case 'timestamp':
            case 'timestamp with time zone':
                if ($value === NULL || $value === false) {
                    return $value;
                } elseif (is_int($value)) {
                    return date('r', $value );
                } elseif (is_string($value)) {
                    return date('r', strtotime($value));
                } else {
                    return $value;
                }
                break;

            case 'text[]';
                return toPgArray($value);
                break;

            default:
                throw new MintException(sprintf("[%s] Unknown type: '$type'", get_called_class()));
                break;
        }
    }

    protected function loadByRow($row) {

        if(empty($row)) {
            throw new MintLoadException('Failed to load class ' . get_called_class());
        }

        foreach($row as $key => $value) {

            $this->$key = $value;
        }

        $this->loaded = true;

        return true;
    }

    public function load() {

        $dbo = MintConnectionPool::getInstance()->getDBO($this->connection_id);

        $row = $dbo->getRow("SELECT * FROM {$this->table} WHERE {$this->pk_field} = ? LIMIT 1",
                array(self::toDBType($this->pk_type, $this->pk_value)), PDO::FETCH_ASSOC);

        return $this->loadByRow($row);
    }

    public function delete() {

        $dbo = MintConnectionPool::getInstance()->getDBO($this->connection_id);

        $dbo->query("DELETE FROM {$this->table} WHERE {$this->pk_field} = ?",
                array(self::toDBType($this->pk_type, $this->pk_value)));

        $this->loaded = false;

        return true;
    }

    public function save() {

        $dbo = MintConnectionPool::getInstance()->getDBO($this->connection_id);

        if(!$this->initialized) {
            $this->initFields();
        }

        if(!is_null($this->pk_value) && $this->loaded) {
            // UPDATE

            $field_list = array();

            foreach($this->fields as $name => $param) {
                if(isset($param['readonly']) && $param['readonly']) {
                    continue;
                }
                $field_list[] = "$name = ?";
            }

            $field_list[] = "{$this->pk_field} = ?";

            $field_list = implode(', ', $field_list);

            $stmt = "UPDATE {$this->table} SET " . $field_list . " WHERE " . $this->pk_field . " = ?";

            $values = array();
            foreach($this->map as $field => $data) {
                $values[] = self::toDBType($data['type'], $data['value']);
            }

            if($dbo->engine == 'mysql') {
                $values[] = self::toDBType($this->pk_type, is_null($this->__pk_value_old) ? $this->pk_value : $this->__pk_value_old);
            }

            $this->__last_op = 'update';

        } else {
            // INSERT

            if(empty($this->pk_value)) {
                if($dbo->engine == 'pgsql' && !empty($this->sequence)) {
                    $this->pk_value = $this->nextId();
                    $this->map[$this->pk_field]['value'] = $this->pk_value;
                }
            } else {
                if(!is_numeric($this->pk_value) && !is_null($this->pk_value)) {
                    $this->map[$this->pk_field]['type'] = 'varchar';
                }
            }

            $no_ro_fields = $this->fields;

            if($dbo->engine == 'pgsql') {
                $no_ro_fields[$this->pk_field] = $this->map[$this->pk_field];
            }

            foreach($no_ro_fields as $field => $param) {
                if(isset($param['readonly']) && $param['readonly']) {
                    unset($no_ro_fields[$field]);
                }
            }

            $field_list = implode(', ', array_keys($no_ro_fields));

            $qs = implode(', ', array_fill(0, count(array_keys($no_ro_fields)), '?'));

            $stmt = "INSERT INTO {$this->table} ({$field_list}) VALUES ({$qs})";

            $values = array();
            foreach($no_ro_fields as $field => $data) {
                if($field != $this->pk_field) {
                    $values[] = self::toDBType($data['type'], $this->map[$field]['value']);
                }
            }

            $this->__last_op = 'insert';
        }

        if($dbo->engine == 'pgsql') {
            $values[] = self::toDBType($this->pk_type, is_null($this->__pk_value_old) ? $this->pk_value : $this->__pk_value_old);
        }

        try {

            $res = $dbo->query($stmt, $values);

        } catch (PDOException $pdo_e) {

            MintLogger::getInstance()->error($pdo_e->getMessage());
            throw new MintSaveException('Failed to save class ' . get_called_class());
        }

        if($dbo->engine == 'mysql') {

            if($this->__last_op == 'insert') {
                $this->{$this->getPKField()} = $this->pk_type == 'integer' ?
                        (integer)$dbo->getLastInsertId() :
                        $dbo->getLastInsertId();
            }
        }

        return true;
    }

    public function asArray() {

        $array = array();

        foreach($this->map as $field => $data) {

            $array[$field] = $data['value'];
        }

        return $array;
    }

    public static function getByPK($pk_value) {

        $self = new static();
        $self->pk_value = $pk_value;
        $self->load();
        return $self;
    }

    public static function getByName($name, $name_field = 'name') {

        $params = array();
        $params[] = array("$name_field = ?", array($name));

        return self::getOne($params);
    }

    public static function getOne($params = array(), $offset = 0, $sort = '', $table = null) {

        $result = self::getByParams($params, $offset, 1, $sort, $table);

        return count($result) > 0 ? $result[0] : null;
    }

    public static function getAll($sort = '', $table = null) {

        return self::getByParams(array(), 0, -1, $sort, $table);
    }

    public static function getByParams($params = array(), $offset = 0, $limit = -1, $sort = '', $table = null) {

        $offset = intval($offset);
        $limit = intval($limit);

        if($offset < 0) {
            $offset = 0;
        }

        $values = array();

        $self = new static();
        $self->table = !empty($table) ? $table : $self->table;

        $dbo = MintConnectionPool::getInstance()->getDBO($self->connection_id);

        $query = "SELECT * FROM {$self->table} ";

        foreach($params as $iter => $param) {

            if($iter == 0) {
                $query .= "WHERE ";
            }

            $clause = ($iter > 0) ? (isset($param[2]) ? $param[2] : 'AND') : '';

            $query .= " $clause " . $param[0];

            if(isset($param[1])) {
                if(is_array($param[1])) {
                    $values = array_merge($values, $param[1]);
                } else {
                    $values[] = $param[1];
                }
            }
        }

        if(!empty($sort)) {

            $order = 'ASC';

            if(strpos($sort, '-') === 0) {
                $order = 'DESC';
                $sort = substr($sort, 1);
            }

            if(!in_array($sort, array_keys($self->fields))) {
                $sort = "{$self->pk_field}";
            }

            $query .= " ORDER BY {$sort} {$order}";
        }

        if($limit > 0 ) {

            switch($dbo->engine) {
                case 'pgsql':
                    $query .= " OFFSET {$offset} LIMIT {$limit}";
                    break;
                case 'mysql':
                    $query .= " LIMIT {$limit} OFFSET {$offset}";
                    break;
            }
        }

        $rows = $dbo->fetchAll($query, array_values($values));

        $result = array();
        foreach($rows as $row) {
            $self = new static();
            $self->loadByRow($row);
            $result[] = $self;
        }

        return $result;
    }

    public static function getCount($params = array(), $offset = 0, $limit = -1, $table = null) {

        $offset = intval($offset);
        $limit = intval($limit);

        $values = array();

        $self = new static();
        $self->table = !empty($table) ? $table : $self->table;

        $dbo = MintConnectionPool::getInstance()->getDBO($self->connection_id);

        $query = "SELECT COUNT(*) FROM {$self->table} ";

        if(!is_array($params)) {
            $params = array($params);
        }

        foreach($params as $iter => $param) {

            if($iter == 0) {
                $query .= "WHERE ";
            }

            $clause = ($iter > 0) ? (isset($param[2]) ? $param[2] : 'AND') : '';

            $query .= " $clause " . $param[0];

            if(is_array($param[1])) {
                $values = array_merge($values, $param[1]);
            } else {
                $values[] = $param[1];
            }
        }

        if(!empty($sort)) {

            $order = 'ASC';

            if(strpos($sort, '-') === 0) {
                $order = 'DESC';
                $sort = substr($sort, -1, -1);
            }

            if(!in_array($sort, array_keys($self->fields))) {
                $sort = "{$self->pk_field}";
            }

            $query .= " ORDER BY {$sort} {$order}";
        }

        if($limit > 0) {
            $query .= " OFFSET {$offset} LIMIT {$limit}";
        }

        $count = $dbo->getOne($query, $values);

        return $count;
    }

    public static function exists($id) {

        try {

            $self = self::getByPK($id);

        } catch (MintLoadException $e) {

            return false;
        }

        return true;
    }

}
