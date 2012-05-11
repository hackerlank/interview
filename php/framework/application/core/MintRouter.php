<?php

class MintRouter extends Singleton{

    public static function getControllerName() {

        if(isset($params['command'])) {

            $params['command'] = trim($params['command'], '/');

            if(strpos($params['command'], '/') !== false) {

                return current(explode('/', $params['command'], 2));

            } else {

                return $params['command'];
            }
        }

        return MINT_CONTROLLER_DEFAULT_NAME;
    }

    public static function getControllerMethodName() {

        $params = $_REQUEST;

        if(isset($params['command'])) {

            $params['command'] = trim($params['command'], '/');

            if(strpos($params['command'], '/') !== false) {
                return next(explode('/', $params['command']));
            }
        }

        return MINT_CONTROLLER_DEFAULT_METHOD_NAME;
    }

    public static function getControllerMethodParams() {

        $params = $_REQUEST;

        if(isset($params['command'])) {

            $params['command'] = trim($params['command'], '/');

            if(strpos($params['command'], '/') !== false) {

                $array = explode('/', $params['command']);

                if(count($array) > 2) {
                    array_shift($array); // deleting controller name
                    array_shift($array); // deleting method name
                    return $array;
                }
            }
        }

        return MINT_CONTROLLER_DEFAULT_METHOD_PARAMS;
    }

    /**
     * @returns list ($controller_name, $method_name, $method_params)
     */
    public static function getControllerData() {

        $params = $_REQUEST;

        $array = array();
        $array[0] = MINT_CONTROLLER_DEFAULT_NAME;
        $array[1] = MINT_CONTROLLER_DEFAULT_METHOD_NAME;
        $array[2] = MINT_CONTROLLER_DEFAULT_METHOD_PARAMS;
        $array[3] = MINT_CONTROLLER_DEFAULT_TEMPLATE_NAME;

        if(isset($params['command'])) {

            $params['command'] = strtolower(trim($params['command'], '/'));
            $array[0] = 'Controller' . ucfirst($params['command']);

            if(strpos($params['command'], '/') !== false) {

                $tmp = explode('/', $params['command']);
                $array[0] = 'Controller' . ucfirst($tmp[0]);
                $array[1] = $tmp[1];

                if(count($tmp) > 2) {
                    array_shift($tmp);
                    array_shift($tmp);
                    $array[2] = $tmp;
                }
            }
        }

        $array[3] = str_replace('controller', '', strtolower($array[0])) . DIRECTORY_SEPARATOR . strtolower($array[1]);

        return $array;
    }

    public static function templateExists($tpl) {

        $tpls_dir = MintINI::getInstance()->getValue('smarty_template_dir');
        return file_exists($tpls_dir . $tpl . MINT_SMARTY_TEMPLATE_POSTFIX);
    }

    public static function checkACL($controller_name, $controller_method) {

        $user = MintUser::getCurrentUser();
        $acl = MintACLItem::getByController($controller_name, $controller_method);

        if(is_null($acl)) {
            throw new MintUnexpectedSituation(sprintf('ACL not found for route %s/%s', $controller_name, $controller_method));
        }

        $result = null;

        if($user instanceof MintAnonymousUser) {

            $result = $acl->bit_position == 0;

        } elseif($user instanceof MintManager) {

            if($user->is_superuser) {

                $result = $user->is_active;

            } else {

                $bit = intval($user->acl[intval($acl->bit_position)]);
                $result = ($bit == 1) && $user->is_active;
            }

        } else {

            $bit = intval($user->acl[intval($acl->bit_position)]);
            $result = ($bit == 1) && $user->is_active;
        }

        return $result;
    }
}