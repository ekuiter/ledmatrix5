<?php

require_once "config.inc.php";

header("Content-Type: text/plain");

$usage = "USAGE:
api.php?state
api.php?run/effect[/color[/argument]]

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
    echo (new LedMatrix(MATRIX_HOST, MATRIX_READ))->getState();
  else if ($query[0] === "run") {
    if (!isset($query[1]) || empty($query[1]))
      die($usage);
    $effect = $query[1];
    if (isset($query[2]) && !empty($query[2]))
      $color = $query[2];
    else $color = COLOR_UNDEF;
    if (isset($query[3]) && !empty($query[3]))
      $argument = $query[3];
    else $argument = -1;
    (new Effect($effect))->run($color, $argument);
  } else
    die($usage);
} catch (Exception $e) {
  die($e->getMessage());
}