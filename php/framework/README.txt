

    Requirements:

        * PHP 5.3 with CLI support.
        * Well-configured apache with installed mod_php, mod_rewrite.
        * php-pdo and php-pdo-pgsql extensions.
        * php5-simplexml support.
        * SSL support.
        * Well-configured PostgreSQL-server 8.x or 9.x
        * Mint apache virtual-host must contain AllowOverride All directive.

    Application structure:

        * common/        - Common classes, patterns, algorythms, etc.
        * conf/          - Sample configuration.
        * controllers/   - Yep, its controllers of MVC.
        * core/          - Core classes, such as ActiveRecord, INI, Router, etc.
        * doc/           - Documentation.
        * externals/     - External libraries/frameworks. E.g: Smarty.
        * include/       - Bootstrap files.
        * install/       - Installation files.
        * models/        - Yep, its models of MVC.
        * tpls/          - Yep, its templates (views) of MVC.
        * upgrade/       - Upgrade files, they are used when updating/upgrading.

        * .htaccess      - .htaccess configuration file. Various rewrites and settings.
        * INSTALL.txt    - Installation manual
        * README.txt     - This file.
        * index.php      - Entry point of the application. Accessible only thru HTTP.
        * install.php    - Installation script. CLI ONLY.
        * manage.php     - Administration script. CLI ONLY.
        * TODO.txt       - TODO list.


