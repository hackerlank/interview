<?php

class ControllerModel extends MintController implements MintControllerInterface {

    protected $page_title = 'Models';

    public function index($params) {

    }

    public function add($params) {

        $model = new MintCustomModelType();

        $model->name = $params['name'];
        $model->name_ru = $params['name_ru'];
        $model->table = $params['table'];

        $model->save();

        $xml = new SimpleXMLElement('<?xml encoding="utf-8" version="1.0"?><mutation></mutation>');
        $xml->addChild('comment', sprintf('Create table "%s"', $model->name));
        $xml->addChild('evolve_sql', sprintf('CREATE TABLE "custom"."%s" (
            id serial,
            created timestamp not null default now()
            
        ) WITH (OIDS=FALSE);'));

    }
}