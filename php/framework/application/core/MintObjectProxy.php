<?php

abstract class MintObjectProxy extends MintActiveRecord {

    public function save() {

        $old_pk = $this->__pk_value_old;

        parent::save();

        // Отдельно сохраняем в таблицу objects
        $class = get_called_class();

        if(!MintObject::exists($this->getPKValue(), $class)) {

            if($this->__last_op == 'insert') {

                $mint_object = new MintObject();
                $mint_object->model = $class;
                $mint_object->foreign_id = $this->getPKValue();
                $mint_object->save();

            } elseif($this->__last_op == 'update') {

                if(MintObject::exists($old_pk, $class)) {

                    $mint_object = MintObject::getByForeignId($class, $old_pk);
                    $mint_object->foreign_id = $this->getPKValue();
                    $mint_object->save();

                } else {

                    throw new MintIntegrityError(sprintf('Can\'t update MintObject of model %s with pk=%s. MintObject does not exist',
                            $class, $old_pk));
                }

            } else {

                throw new MintUnexpectedSituation('Invalid __last_op value: ' . $$this->__last_op);
            }
        }
    }

    public function delete() {

        parent::delete();

        $class = get_called_class();

        if(MintObject::exists($this->getPKValue(), $class)) {

            $mint_object = MintObject::getByForeignId($class, $this->getPKValue());
            $mint_object->delete();

        } else {

            throw new MintIntegrityError(sprintf('MintObject does not exist for model %s ; PK %s', $class, $this->getPKValue()));
        }
    }

    public function linkAsChild(MintObject $parent) {

        if(!MintObjectLink::exists($parent->getPKValue(), $this->getPKValue())) {

            $link = new MintObjectLink();
            $link->parent_id = $parent->getPKValue();
            $link->child_id = $this->getPKValue();
            $link->save();

        } else {

            throw new MintIntegrityError(sprintf('Objects already linked: %s,%s', $parent->getPKValue(), $this->getPKValue()));
        }
    }

    public function linkAsParent(MintObject $child) {

        if(!MintObjectLink::exists($this->getPKValue(), $child->getPKValue())) {

            $link = new MintObjectLink();
            $link->parent_id = $this->getPKValue();
            $link->child_id = $child->getPKValue();
            $link->save();

        } else {

            throw new MintIntegrityError(sprintf('Objects already linked: %s,%s', $this->getPKValue(), $child->getPKValue()));
        }
    }

    public static function getByObjectId($object_id) {

        if(MintObject::exists($object_id)) {

            $mint_object = MintObject::getByPK($object_id);

            $class = $mint_object->model;

            return $class::getByPK($mint_object->foreign_id);

        } else {

            throw new MintException(sprintf('Object with id %s does not exist', $object_id));
        }
    }
    

}