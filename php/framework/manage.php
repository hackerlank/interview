<?php

require_once 'include/bootstrap.php';

if(MINT_RUNNING_MODE != MINT_MODE_CLI) {
    throw new Exception("manage.php script is CLI-only");
}

define('DEFAULT_SCHEMA_FILE', 'install/schema.evolution.xml');
define('DEFAULT_TEST_DATA_FILE', 'install/testdata.evolution.xml');

/*
 * usage: php manage.php <command> [args]
 */

if($argc < 2) {
    print "Usage: manage.php <command> [args]" . PHP_EOL;
    print "Commands:" . PHP_EOL;
    print "  help [command]" . "\t" . " Shows help page" . PHP_EOL;
    print "  evolve <file>" . "\t\t" . " Runs SQL evolution script" . PHP_EOL;
    print "  rollback <file>" . "\t" . " Rollbacks SQL evolution script" . PHP_EOL;
    print "  deploy" . "\t\t" . " Deploys (re-deploys) database schema, installs testing data" . PHP_EOL;
    exit;
}

$command = trim(strtolower($argv[1]));

if(!function_exists($command)) {
    print "Unknown command" . PHP_EOL;
} else {
    $command();
}


function evolve($filename = null) {

    global $argv;

    if(!isset($argv[2]) && is_null($filename)) {
        print "Usage: manage.php evolve <file>" . PHP_EOL;
        exit;
    }

    if(is_null($filename)) {
        $filename = $argv[2];
    }

    $evolution = MintEvolution::getFromFile($filename);

    try {

        $evolution->evolve();

    } catch (MintEvolutionException $e) {

        print "Can't apply evolution. Error is: " . $e->getMessage() . PHP_EOL;
        print "SQL was: " . $e->getSQL() . PHP_EOL;
    }
}

function rollback($filename = null) {
    
    global $argv;
    
    if(!isset($argv[2]) && is_null($filename)) {
        print "Usage: manage.php rollback <file>" . PHP_EOL;
        exit;
    }

    if(is_null($filename)) {
        $filename = $argv[2];
    }

    $evolution = MintEvolution::getFromFile($filename);

    try {

        $evolution->rollback();

    } catch (MintEvolutionException $e) {

        print "Can't apply evolution. Error is: " . $e->getMessage() . PHP_EOL;
        print "SQL was: " . $e->getSQL() . PHP_EOL;
    }
}

function deploy() {

    global $argv;

    if(MintRegistryEntry::exists('db_initialized')) {
        rollback('install/testdata_mysql.evolution.xml');
        rollback('install/schema_mysql.evolution.xml');
        rollback(DEFAULT_TEST_DATA_FILE);
        rollback(DEFAULT_SCHEMA_FILE);
    }

    evolve(DEFAULT_SCHEMA_FILE);
    evolve(DEFAULT_TEST_DATA_FILE);
    evolve('install/schema_mysql.evolution.xml');
    evolve('install/testdata_mysql.evolution.xml');
}

