<?php

class State {
  public static function get() {
    if (!file_exists($GLOBALS["stateFile"]))
      return array(
        "mode" => "plugin",
        "plugin" => "idle"
      );
    return unserialize(file_get_contents($GLOBALS["stateFile"]));
  }

  public static function isPlugin($plugin) {
    $state = self::get();
    if ($state["mode"] != "plugin")
      return NULL;
    return $state["plugin"] == $plugin->pluginName;
  }

  private static function set($state) {
    file_put_contents($GLOBALS["stateFile"], serialize($state));
  }

  public static function setManual() {
    self::set(array(
      "mode" => "manual"
    ));
  }

  public static function setPlugin($plugin) {
    self::set(array(
      "mode" => "plugin",
      "plugin" => $plugin->pluginName
    ));
  }
}