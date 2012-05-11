<?php

class MintLogger extends Singleton {

    const LOG_INFO = 1;
    const LOG_ERROR = 2;

    public function log($message, $mode = self::LOG_INFO) {

        switch($mode) {

            default:
            case self::LOG_INFO:

                break;

            case self::LOG_ERROR:

                $message = sprintf("[%s] %s", "ERROR", $message);
                break;
        }

        switch(MINT_RUNNING_MODE) {

            case MINT_MODE_WWW:

                // @todo: some loggin to database, emails, etc...

                break;

            case MINT_MODE_CLI:

                print $message . PHP_EOL;
                break;

            default:

                throw new MintUnexpectedSituation('Unknown MINT_RUNNING_MODE');
                break;
        }
    }
}