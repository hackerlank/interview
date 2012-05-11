<?php

define('USE_OS', false);

require_once 'include/bootstrap.php';

list($controller_name, $controller_method, $controller_params, $tpl_name) = MintRouter::getControllerData();

if(!is_callable(array($controller_name, $controller_method))/* || !MintRouter::templateExists($tpl_name)*/) {

    $controller_name = MINT_CONTROLLER_DEFAULT_NAME;
    $controller_method = 'error404';
    $tpl_name = 'index/error404';
}

if(!MintRouter::checkACL($controller_name, $controller_method)) {

    $controller_name = MINT_CONTROLLER_DEFAULT_NAME;
    $controller_method = 'noaccess';
    $tpl_name = 'index/noaccess';
}

$controller = $controller_name::getInstance();
$controller->$controller_method($controller_params);
$controller->assign('USE_OS', USE_OS);
$controller->assign('include', $tpl_name);
$controller->assign('menu', MintMenu::getByUser(MintUser::getCurrentUser()));
$controller->render('main');