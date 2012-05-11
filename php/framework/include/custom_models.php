<?php

if(MintRegistryEntry::exists("db_initialized")) {

    foreach(MintCustomModelType::getAll() as $custom_model) {

        // @todo: некрасиво
        //class_alias('MintCustomModel', $custom_model->model);
        eval("class {$custom_model->model} extends MintCustomModel {}");
    }
}
