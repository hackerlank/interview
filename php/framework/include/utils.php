<?php

function var_dump_pre() {
    $args = func_get_args();
    echo '<pre>';
    call_user_func_array('var_dump',$args);
    echo '</pre>';
}