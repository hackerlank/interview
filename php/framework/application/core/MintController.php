<?php

interface MintControllerInterface {

    public function index($params);
    public function render($tpl);
}

abstract class MintController extends Singleton implements MintControllerInterface {

    protected $view = array();
    protected $errors = array();
    protected $smarty;
    protected $page_title;
    protected $is_ajax = false;

    public function __construct() {
        $this->smarty = MintSmarty::getInstance();
        if(isset($_SERVER['HTTP_X_REQUESTED_WITH'])) {
            $this->is_ajax = true;
        }
    }

    public function assign($key, $value) {

        $this->view[$key] = $value;
    }

    public function error($text) {

        $this->errors[] = $text;
    }

    public function render($tpl) {

        $this->assign('PAGE_GENERATION_TIME', sprintf("%.3f", microtime(true) - MINT_STARTUP_TIME));
        $this->assign('MINT_APPLICATION_NAME', MINT_APPLICATION_NAME);
        $this->assign('MINT_PAGE_TITLE', $this->page_title);
        $this->assign('errors', $this->errors);

        if(isset($_REQUEST['json'])) {

            header('Content-Type: application/json; charset=' . MINT_INTERNAL_ENCODING);

            $json = new MintResponseJSON();
            $json->request_time = $_SERVER['REQUEST_TIME'];
            $json->request_uri = $_SERVER['REQUEST_URI'];
            $json->errors = $this->errors;
            $json->success = count($this->errors) == 0;
            $json->data = $this->view;

            print $json->serialize();

        } else {

            if(!$this->is_ajax) {

                $this->smarty->assign($this->view);
                $this->smarty->display($tpl . MINT_SMARTY_TEMPLATE_POSTFIX);

            } else {

                $this->smarty->assign('ajax_data', json_encode($this->view));
                $this->smarty->display(MINT_SMARTY_TEMPLATE_JSON . MINT_SMARTY_TEMPLATE_POSTFIX);
            }
        }


    }
}