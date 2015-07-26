<?php

require_once "config.inc.php";

if (!isset($_GET["controlPass"]) || $_GET["controlPass"] !== $GLOBALS["controlPass"])
  die("Unauthorized access");

$ob_file = fopen($GLOBALS["logFile"], "w");

function ob_file_callback($buffer) {
  global $ob_file;
  fwrite($ob_file, $buffer);
}

ob_start("ob_file_callback");

$control->loop();