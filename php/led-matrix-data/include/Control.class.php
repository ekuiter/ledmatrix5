<?php

class Control {
  private $plugins, $idlePlugin;

  function __construct($plugins) {
    $this->plugins = $plugins;
    $this->idlePlugin = new IdlePlugin();
  }

  public function loop() {
    $start = microtime(true);

    for ($i = 1; ; $i++) {
      foreach ($this->plugins as $plugin) {
        $state = State::get();
        if ($state->isManual()) {
          if ($state->isSwitching()) {
            echo "[" . date("d.m.Y H:i:s") . "]    Switching to manual mode (effect: {$state->getEffect()}).\n";
            $state->setSwitching(false);
          }
        } else {
          if ($state->isSwitching()) {
            echo "[" . date("d.m.Y H:i:s") . "]    Switching to plugin mode.\n";
            $this->idlePlugin->displayData();
          }
          try {
            if ($plugin->isInvalid()) {
              $data = $plugin->fetchData();
              $isNewData = $plugin->isNewData($data);
              $isRelevantData = $plugin->isRelevantData($data);
              if ($isRelevantData && $isNewData)
                $plugin->displayData($data);
              if (State::get()->isPlugin($plugin) && !$isRelevantData)
                $this->idlePlugin->displayData();
              $label = $isNewData ? " !" : "=>";
              $label = $isRelevantData ? $label : " .";
              echo "[" . date("d.m.Y H:i:s") . "] $label $plugin->pluginName: " . $plugin->stringifyData($data) . "\n";
              $plugin->storeData($data);
            }
          } catch (PluginException $e) {
            echo "[" . date("d.m.Y H:i:s") . "]    $plugin->pluginName: " . $e->getMessage() . "\n";
          }
        }
      }

      ob_flush();
      @time_sleep_until($start + $i);
    }
  }

  public function log() {
    $numberOfLines = 30;

    header("Content-Type: text/plain");
    $status = $this->runCommand("status");
    $return = "$status\n";
    if (!file_exists($GLOBALS["logFile"]))
      throw new Exception("No log file found!");
    if (stristr($status, "not running"))
      throw new Exception("Control script is not running");

    $lines = array();
    $fp = fopen($GLOBALS["logFile"], "r");
    while (!feof($fp)) {
      $line = fgets($fp, 4096);
      $lines[] = $line;
      if (count($lines) > $numberOfLines)
        array_shift($lines);
    }
    fclose($fp);

    $return .= implode("", $lines);
    return $return;
  }

  public function runCommand($command) {
    $commands = array("start", "stop", "reset", "status", "log");
    if (!in_array($command, $commands))
      throw new InvalidArgumentException("invalid command");
    if ($command == "start")
      State::setPlugin("idle", true);
    else if ($command == "reset")
      return `reset-mcu`;
    else if ($command == "log")
      return $this->log();
    else
      return `/etc/init.d/led-matrix $command`;
    return "";
  }
}