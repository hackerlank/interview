<?php

require_once 'include/constants.php';
require_once 'core/MintSysReqException.php';

define('INSTALL_SCHEMA_FILE', 'install/schema.evolution.xml');

try {

    require_once 'include/bootstrap.php';

    if(MINT_RUNNING_MODE != MINT_MODE_CLI) {
        throw new Exception("install.php script is CLI-only");
    }

    if(version_compare(PHP_VERSION, MINT_SYSREQ_PHP_VERSION) < 0) {
        throw new MintSysReqException("PHP version must be at least " . MINT_SYSREQ_PHP_VERSION);
    }

    foreach(array('libxml', 'PDO', 'pdo_pgsql', 'pgsql', 'Reflection', 'SimpleXML', 'xml') as $extension) {

        if(!extension_loaded($extension)) {
            throw new MintSysReqException(sprintf("PHP %s extension not installed", $extension));
        }
    }

    $evolution = MintEvolution::getFromFile(INSTALL_SCHEMA_FILE);

    try {

        $evolution->evolve();

    } catch (MintEvolutionException $e) {

        fwrite(STDERR, "Can't apply evolution. Error is: " . $e->getMessage() . PHP_EOL);
        fwrite(STDERR, "SQL was: " . $e->getSQL() . PHP_EOL);
        throw new Exception("Failed to install database schema");
    }

    /*fwrite(STDOUT, "db_host: ");
    fscanf(STDIN, "%s\n", $db_host);

    fwrite(STDOUT, "db_name: ");
    fscanf(STDIN, "%s\n", $db_name);

    fwrite(STDOUT, "db_user: ");
    fscanf(STDIN, "%s\n", $db_user);

    fwrite(STDOUT, "db_pass: ");
    fscanf(STDIN, "%s\n", $db_pass);

    fwrite(STDOUT, "document root: ");
    fscanf(STDIN, "%s\n", $document_root);

    fwrite(STDOUT, "smarty_compile_dir: ");
    fscanf(STDIN, "%s\n", $smarty_compile_dir);

    fwrite(STDOUT, "smarty_cache_dir: ");
    fscanf(STDIN, "%s\n", $smarty_cache_dir);

    fscanf(STDOUT, "Where to store config file: ");
    fscanf(STDIN, "%s\n", $config_file);

    fwrite(STDOUT, "Saving config file")*/

    fwrite(STDOUT, "Success" . PHP_EOL);

} catch (MintSysReqException $sys_e) {

    fwrite(STDERR, sprintf("System requirements violation: %s", $sys_e->getMessage()) . PHP_EOL);
    fwrite(STDERR, sprintf("For all requirements list see README.txt or type \"php manage.php req\"") . PHP_EOL);

} catch (Exception $e) {

    fwrite(STDERR, $e->getMessage());
}

