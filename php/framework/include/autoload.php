<?php

function __autoload($class_name) {

    $class_name = str_replace('/', '_', $class_name);
    $file_name = $class_name . '.php';

    // Doing this due to performance issues.
    // I think it's faster than looping thru get_include_path() and checking whether the file exists or not.
    // Failing is okay as long as it tries to load controller - controller may exist or may not exist, we don't care.
    // The problem begins if include_once fails at loading core/common/models class.
    include_once $file_name;

    // Now let's hope for the best.
    // is_callable($class_name) will return false, so its okay.
}

spl_autoload_register('__autoload');