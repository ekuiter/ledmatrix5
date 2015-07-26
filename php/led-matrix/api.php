<?php

require_once "config.inc.php";

header("Content-Type: text/plain");

$usage = "USAGE:
api.php?state
api.php?ledState
api.php?control/command
api.php?run/effect[/color[/argument]]

ALLOWED COMMANDS:
  stop
  status
  log

ALLOWED EFFECTS:\n";
foreach (unserialize(EFFECTS) as $effectName => $effectId)
  $usage .= "  $effectName\n";
$usage .= "\nALLOWED COLORS:\n";
foreach (unserialize(COLORS) as $colorName => $colorId)
  $usage .= "  $colorName\n";
$usage .= "\nAn additional argument is only allowed for the TEXT effect.";

$queryString = $_SERVER["QUERY_STRING"];
$query = explode("/", $queryString);

try {
  if ($queryString == "state")
    echo json_encode(State::get());
  else if ($queryString == "ledState")
    echo (new LedMatrix(MATRIX_HOST, MATRIX_READ))->getLedState();
  else if ($query[0] === "control") {
    if (!isset($query[1]) || empty($query[1]))
      die($usage);
    echo $control->runCommand($query[1]);
  } else if ($query[0] === "run") {
    if (!isset($query[1]) || empty($query[1]))
      die($usage);
    $effect = $query[1];
    if (isset($query[2]) && !empty($query[2]))
      $color = $query[2];
    else $color = COLOR_UNDEF;
    if (isset($query[3]) && !empty($query[3]))
      $argument = $query[3];
    else $argument = -1;
    $control->runCommand("stop");
    State::setManual();
    (new Effect($effect))->run($color, $argument);
  } else
    die($usage);
} catch (Exception $e) {
  die($e->getMessage());
}