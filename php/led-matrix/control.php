<?php

require_once "config.inc.php";

$ob_file = fopen($GLOBALS["logFile"], "w");

function ob_file_callback($buffer) {
  global $ob_file;
  fwrite($ob_file, $buffer);
}

ob_start("ob_file_callback");

$control->loop();