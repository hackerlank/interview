<?php

class ControllerAccount extends MintController implements MintControllerInterface {

    protected $page_title = 'Authorization';

    public function index($params) {

    }

    public function login($params) {

        if(!(MintUser::getCurrentUser() instanceof MintAnonymousUser)) {

            header('Location: /');

        } else {

            if(!empty($_POST)) {

                $login = $_POST['login'];
                $password = md5(MINT_PASSWORD_SALT . $_POST['password']);

                if(($manager = MintManager::getByAuth($login, $password)) instanceof MintManager) {

                    if($manager->is_active) {

                        $session_id = uniqid();

                        $session = new MintSession();
                        $session->manager_id = $manager->id;
                        $session->cookie = $session_id;
                        $session->ip = $_SERVER['REMOTE_ADDR'];
                        $session->valid_till = strtotime(sprintf("+%s seconds", MintINI::getInstance()->getValue('session_lifetime')));
                        $session->save();

                        setcookie(MINT_SESSION_NAME, $session_id, time() + MintINI::getInstance()->getValue('session_lifetime'), '/');
                        header('Location: /');

                    } else {

                        $this->error("Sorry, access denied");
                    }

                } elseif(($client = MintClient::getByAuth($login, $password)) instanceof MintClient) {

                    if($client->is_active) {

                        $session_id = uniqid();

                        $session = new MintSession();
                        $session->client_id = $client->id;
                        $session->cookie = $session_id;
                        $session->ip = $_SERVER['REMOTE_ADDR'];
                        $session->valid_till = strtotime(sprintf("+%s seconds", MintINI::getInstance()->getValue('session_lifetime')));
                        $session->save();

                        setcookie(MINT_SESSION_NAME, $session_id, time() + MintINI::getInstance()->getValue('session_lifetime'), '/');
                        header('Location: /');

                    } else {

                        $this->error("Sorry, access denied");
                    }

                } else {

                    $this->error("Authorization failed");
                }
            }
        }
    }

    public function logout($params) {

        $session = MintSession::getByCookie($_COOKIE[MINT_SESSION_NAME]);
        $session->delete();
        setcookie(MINT_SESSION_NAME, "", time()-3600);
        header('Location: /');
    }

    public function register($params) {

    }
}