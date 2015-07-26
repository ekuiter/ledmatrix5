<?php

require_once "config.inc.php";

$logFile = "$ledMatrixDataPath/control.log";
$numberOfLines = 30;

header("Content-Type: text/plain");
echo `/etc/init.d/led-matrix status` . "\n";
if (!file_exists($logFile))
  die("No log file found!");

$lines = array();
$fp = fopen($logFile, "r");
while (!feof($fp)) {
  $line = fgets($fp, 4096);
  $lines[] = $line;
  if (count($lines) > $numberOfLines)
    array_shift($lines);
}
fclose($fp);

echo implode("", $lines);