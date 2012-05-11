<?php

class ControllerIndex extends MintController implements MintControllerInterface {

    protected $page_title = 'Index';

    public function index($params) {

        if(MintUser::getCurrentUser() instanceof MintAnonymousUser) {
            header('Location: /account/login');
        }
    }
    
    public function noaccess($params) {

    }

    public function error404() {

        header("HTTP/1.1 404 Not Found");
    }

    public function error403($params) {

        header("HTTP/1.1 403 Forbidden");
    }
}