<?php

abstract class Plugin {
  public $pluginName;
  private $interval, $store, $lastStored;

  function __construct($pluginName, $interval) {
    $this->pluginName = $pluginName;
    $this->store = "$pluginName.dat";
    $this->interval = $interval;
  }

  abstract public function fetchData();
  abstract public function isNewData($data);
  abstract public function isRelevantData($data);
  abstract public function stringifyData($data);
  abstract public function displayData($data, $ledMatrix);

  public function loadData() {
    if (file_exists($this->store))
      return unserialize(file_get_contents($this->store));
    else
      return array();
  }

  public function storeData($data) {
    $this->lastStored = time();
    file_put_contents($this->store, serialize($data));
  }

  public function isInvalid() {
    if (!$this->lastStored)
      return true;
    return time() >= $this->lastStored + $this->interval;
  }
}

class PluginException extends Exception {}