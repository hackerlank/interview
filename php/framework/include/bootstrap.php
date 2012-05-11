<?php

define('MINT_MODE_CLI', 1);
define('MINT_MODE_WWW', 2);
define('MINT_RUNNING_MODE', PHP_SAPI == 'cli' ? MINT_MODE_CLI : MINT_MODE_WWW);

define('MINT_STARTUP_TIME', microtime(true));
define('DOCUMENT_ROOT', realpath(dirname(__FILE__) . '/..'));

set_include_path(get_include_path() . PATH_SEPARATOR . DOCUMENT_ROOT . '/common');
set_include_path(get_include_path() . PATH_SEPARATOR . DOCUMENT_ROOT . '/application/core');
set_include_path(get_include_path() . PATH_SEPARATOR . DOCUMENT_ROOT . '/application/models');
set_include_path(get_include_path() . PATH_SEPARATOR . DOCUMENT_ROOT . '/application/controllers');

require_once 'constants.php';
require_once 'externals.php';
require_once 'errorhandler.php';
require_once 'utils.php';
require_once 'autoload.php';
require_once 'custom_models.php';
require_once 'config.php';
require_once 'session.php';