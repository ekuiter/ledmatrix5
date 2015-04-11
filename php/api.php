<?php

require_once "config.inc.php";

$usage = "USAGE: api.php?getState|run,1,1,1|text,testtext";
$query = explode(",", $_SERVER["QUERY_STRING"]);

try {
  $port = $query[0] === "run" ? MATRIX_WRITE : MATRIX_READ;
  $ledMatrix = new LedMatrix(MATRIX_HOST, $port);
} catch (Exception $e) {
  die($e->getMessage()); 
}

if ($query[0] === "getState")
  echo $ledMatrix->getState(); 
else if ($query[0] === "run") {
  if (!isset($query[1]) || empty($query[1])) 
    die($usage);
  $effect = intval($query[1]);
  if (isset($query[2]) && !empty($query[2]))
    $color = intval($query[2]);
  else
    $color = COLOR_UNDEF;
  if (isset($query[3]) && !empty($query[3]))
    $argument = intval($query[3]);
  else
    $argument = -1;
  $ledMatrix->run($effect, $color, $argument);
} else if ($query[0] === "text") {
  if (!isset($query[1]) || empty($query[1]))
    die($usage);
  $ledMatrix->setText($query[1]);
} else
  die($usage);
