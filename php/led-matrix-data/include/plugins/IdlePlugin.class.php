<?php

class IdlePlugin extends Plugin {

  function __construct() {
    parent::__construct("idle", 0);
  }

  public function fetchData() {}
  public function isNewData($data) {}
  public function isRelevantData($data) {}
  public function stringifyData($data) {}

  protected function displayDataInternal($data) {
    (new Effect("clock"))->run();
  }
}