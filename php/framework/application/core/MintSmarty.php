<?php

final class MintSmarty extends Smarty {

    /**
     * This is bullshit.
     * PHP classes cannot inherit more than 1 (one) class.
     * 
     * @return MintSmarty
     */
    static function getInstance() {

        return Singleton::getInstance(__CLASS__);
    }

    public function __construct() {

        parent::__construct();

        $ini = MintINI::getInstance();

        $this->template_dir = $ini->getValue('smarty_template_dir');
        $this->compile_dir = $ini->getValue('smarty_compile_dir');
        $this->cache_dir = $ini->getValue('smarty_cache_dir');
        $this->config_dir = $ini->getValue('smarty_config_dir');

        $this->left_delimiter = MINT_SMARTY_LEFT_DELIMITER;
        $this->right_delimiter = MINT_SMARTY_RIGHT_DELIMITER;

        $this->debugging = $ini->getValue('smarty_debugging');
        $this->caching = $ini->getValue('smarty_caching');
        $this->compile_check = $ini->getValue('smarty_compile_check');
        $this->force_compile = $ini->getValue('smarty_force_compile');
    }
}