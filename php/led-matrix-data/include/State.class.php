<?php

class State {
  private $state;

  public function __construct($state) {
    $this->state = $state;
  }

  public function getJSON() {
    return json_encode($this->state);
  }

  public function getEffect() {
    if (isset($this->state["effect"]))
      return $this->state["effect"];
    return NULL;
  }

  public function isManual() {
    return $this->state["mode"] == "manual";
  }

  public function isPlugin($plugin) {
    if ($this->state["mode"] != "plugin")
      return NULL;
    return $this->state["plugin"] == $plugin->pluginName;
  }

  public function isSwitching() {
    return $this->state["switching"];
  }

  public function setSwitching($switching) {
    $this->state["switching"] = $switching;
    self::set($this->state);
  }

  public static function get() {
    if (!file_exists($GLOBALS["stateFile"]))
      return new State(array(
        "mode" => "plugin",
        "plugin" => "idle",
        "switching" => true
      ));
    return new State(unserialize(file_get_contents($GLOBALS["stateFile"])));
  }

  private static function set($state) {
    file_put_contents($GLOBALS["stateFile"], serialize($state));
  }

  public static function setManual($effect, $switching = false) {
    self::set(array(
      "mode" => "manual",
      "effect" => $effect,
      "switching" => $switching
    ));
  }

  public static function setPlugin($plugin, $switching = false) {
    self::set(array(
      "mode" => "plugin",
      "plugin" => $plugin->pluginName,
      "switching" => $switching
    ));
  }
}