<?php

foreach(glob(realpath(dirname(__FILE__)) . "/classes/*.php") as $filename) {

    require_once $filename;
}

class AllTests {

    public static function suite() {

        $suite = new PHPUnit_Framework_TestSuite('Mint');

        foreach(glob(realpath(dirname(__FILE__)) . "/classes/*.php") as $filename) {
            $test = basename($filename, '.php');
            $suite->addTestSuite($test);
        }

        return $suite;
    }
}