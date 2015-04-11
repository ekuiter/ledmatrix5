<?php

$ob_file = fopen("control.log", "w");

function ob_file_callback($buffer) {
  global $ob_file;
  fwrite($ob_file, $buffer);
}

ob_start("ob_file_callback");

require_once "../config.inc.php";

$control->loop();