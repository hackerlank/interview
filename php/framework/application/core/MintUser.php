<?php

abstract class MintUser {

    public static function getCurrentUser() {

        static $user = null; // result will be cached

        if(!is_null($user)) {
            return $user;
        }

        if(isset($_COOKIE[MINT_SESSION_NAME])) {

            $session = MintSession::getByCookie($_COOKIE[MINT_SESSION_NAME]);

            try {

                if(!($session instanceof MintSession)) {
                    throw new Exception('Session not found');
                }

                if($session->ip != $_SERVER['REMOTE_ADDR']) {
                    throw new Exception('IP changed');
                }

                if(strtotime($session->valid_till) < time()) {
                    throw new Exception('Session expired');
                }

                if(!is_null($session->client_id)) {

                    $user = MintClient::getByPK($session->client_id);

                } elseif(!is_null($session->manager_id)) {

                    $user = MintManager::getByPK($session->manager_id);

                } else {

                    throw new MintUnexpectedSituation("Session contains empty client_id and manager_id. Session cookie: " . $_COOKIE[MINT_SESSION_NAME]);
                }

            } catch (Exception $e) {

                // Session or client/manager not found. Destroying cookie
                if($session instanceof MintSession) {
                    $session->delete();
                }
                
                setcookie(MINT_SESSION_NAME, "", time()-3600);
                $user = MintAnonymousUser::getInstance();
            }

        } else {

            $user = MintAnonymousUser::getInstance();
        }

        return $user;
    }
}