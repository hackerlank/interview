#!/usr/local/bin/bash

ROOT=/usr/local/www/mint

/usr/local/bin/phpunit --bootstrap ${ROOT}/include/bootstrap.php ${ROOT}/tests/AllTests.php

