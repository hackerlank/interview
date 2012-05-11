<?php

function mint_exception_handler(Exception $exception) {
    echo "Exception: ", $exception->getMessage(), " at ", $exception->getFile(), ":", $exception->getLine(), "<br>";
}

set_exception_handler('mint_exception_handler');

if(MINT_RUNNING_MODE == MINT_MODE_CLI) {
    error_reporting(E_ALL);
    ini_set("display_errors", 1);
}