<?php

class MintACL extends Singleton {

    public static function getByUser($user) {

        static $firsttime = null;
        static $result = null;

        if(!is_null($firsttime)) {
            return $result; // cache
        }

        $firsttime = false;

        // @todo: is_active check?

        if($user instanceof MintAnonymousUser) {

            $result = array();

        } else {

            $acl_params = array();
            $acl_params[] = array('bit_position <> 0');

            if(!($user instanceof MintManager && $user->is_superuser)) {
                // Casting string to array of characters
                foreach((array)$user->acl as $i => $bit) {
                    if($bit == '1') {
                        $acl_params[] = array('bit_position = ?', array($i));
                    }
                }
            }

            $result = MintACLItem::getByParams($acl_params);
        }

        return $result;
    }
}