<?php

date_default_timezone_set(MintINI::getInstance()->getValue("timezone"));
setlocale(LC_ALL, MintINI::getInstance()->getValue("locale"));
