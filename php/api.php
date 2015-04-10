<?php

require_once "auth.inc.php";
require_once "LedMatrix.class.php";

$usage = "USAGE: api.php?getState|run,1,1|text,testtext";
$query = explode(",", $_SERVER["QUERY_STRING"]);

try {
  $port = $query[0] === "run" ? 6571 : 5700;
  $ledMatrix = new LedMatrix("localhost", $port);
} catch (Exception $e) {
  die($e->getMessage()); 
}

if ($query[0] === "getState")
  echo $ledMatrix->getState(); 
else if ($query[0] === "run") {
  if (!isset($query[1]) || empty($query[1])) 
    die($usage);
  $effect = intval($query[1]);
  if (isset($query[2]) || empty($query[2]))
    $color = intval($query[2]);
  else
    $color = NULL;
  $ledMatrix->run($effect, $color);
} else if ($query[0] === "text") {
  if (!isset($query[1]) || empty($query[1]))
    die($usage);
  $context = stream_context_create(array(
    "http" => array(
      "header"  => "Authorization: Basic " . base64_encode("$username:$password")
    )
  ));
  file_get_contents("http://localhost/data/put/text/$query[1]", false, $context);
} else
  die($usage);
