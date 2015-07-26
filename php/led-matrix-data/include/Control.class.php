<?php

class Control {
  private $plugins, $ledMatrix;

  function __construct($plugins) {
    $this->plugins = $plugins;
    $this->ledMatrix = new LedMatrix(MATRIX_HOST, MATRIX_WRITE);
  }

  public function loop() {
    echo "Control started.\n";
    $start = microtime(true);

    for ($i = 1; ; $i++) {
      foreach ($this->plugins as $plugin) {
        try {
          if ($plugin->isInvalid()) {
            $data = $plugin->fetchData();
            $isNewData = $plugin->isNewData($data);
            $isRelevantData = $plugin->isRelevantData($data);
            if ($isRelevantData && $isNewData)
              $plugin->displayData($data, $this->ledMatrix);
            $label = $isNewData ? " !" : "=>";
            $label = $isRelevantData ? $label : " .";
            echo "$label $plugin->pluginName: " . $plugin->stringifyData($data) . "\n";
            $plugin->storeData($data);
          }
        } catch (PluginException $e) {
          echo " x $plugin->pluginName: " . $e->getMessage() . "\n";
        }
      }

      ob_flush();
      @time_sleep_until($start + $i);
    }
  }

  public function log() {
    $logFile = "$GLOBALS[ledMatrixDataPath]/control.log";
    $numberOfLines = 30;

    header("Content-Type: text/plain");
    $status = $this->runCommand("status");
    $return = "$status\n";
    if (!file_exists($logFile))
      throw new Exception("No log file found!");
    if (stristr($status, "not running"))
      throw new Exception("Control script is not running");

    $lines = array();
    $fp = fopen($logFile, "r");
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
      // does not work, see http://stackoverflow.com/questions/566248, help appreciated
      //return `/etc/init.d/led-matrix start`;
      return "";
    else if ($command == "log")
      return $this->log();
    else if ($command == "reset")
      return `reset-mcu`;
    else
      return `/etc/init.d/led-matrix $command`;
  }
}