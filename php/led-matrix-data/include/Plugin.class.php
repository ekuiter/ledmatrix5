<?php

abstract class Plugin {
  public $pluginName;
  private $interval, $store, $lastStored;

  function __construct($pluginName, $interval) {
    $this->pluginName = $pluginName;
    $this->store = "$GLOBALS[dataPath]/$pluginName.dat";
    $this->interval = $interval;
  }

  abstract public function fetchData();
  abstract public function isNewData($data);
  abstract public function isRelevantData($data);
  abstract public function stringifyData($data);
  abstract protected function displayDataInternal($data);

  public function displayData($data = NULL) {
    State::setPlugin($this);
    $this->displayDataInternal($data);
  }

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