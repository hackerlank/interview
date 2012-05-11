<?php

class MintMenu extends Singleton {

    public static function getByUser($user) {

        static $result = null;

        if(!is_null($result)) {
            return $result; // cache
        }

        $acl_list = MintACL::getByUser($user);

        if(!is_null($acl_list)) {

            $menu_params = array();

            foreach($acl_list as $acl_item) {
                $menu_params[] = array('acl_id = ?', array($acl_item->id), 'OR');
            }

            $result = MintMenuItem::getByParams($menu_params);
        }
        
        return $result;
    }
}