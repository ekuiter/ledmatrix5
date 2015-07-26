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
}