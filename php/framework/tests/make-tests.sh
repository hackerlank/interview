#!/usr/local/bin/bash

ROOT=/usr/local/www/mint
MODELS=${ROOT}/application/models
CORE=${ROOT}/application/core
CONTROLLERS=${ROOT}/application/controllers
TESTS=${ROOT}/tests

for filepath in ${MODELS}/* ${CORE}/* ${CONTROLLERS}/*
do
    case $filepath in
	*.php)
	    # PHP FILE
	    filename=`basename $filepath`
	    /usr/local/bin/phpunit --bootstrap ${ROOT}/include/bootstrap.php --skeleton-test ${filename%%.*} ${filename%%.*}.php ${filename%%.*}Test ${TESTS}/classes/${filename%%.*}Test.php
    esac
done;
