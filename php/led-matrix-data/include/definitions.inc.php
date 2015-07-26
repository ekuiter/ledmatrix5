<?php

define("MATRIX_HOST",  "localhost");
define("MATRIX_READ",  5700);
define("MATRIX_WRITE", 6571);

$effect_names = array("loop",
  "idle",
  "light",
  "linearPath",
  "radiantPath",
  "diagonalPath",
  "text",
  "clock");

$color_names = array("random",
  "none",
  "undef",
  "red",
  "green",
  "blue",
  "yellow",
  "magenta",
  "cyan",
  "white");

$effects = array();
for ($i = 0; $i < count($effect_names); $i++) {
  $effect_index = $i == 0 ? -1 : $i;
  $effects[$effect_names[$i]] = $effect_index;
  define("EFFECT_" . strtoupper($effect_names[$i]), $effect_index);
}
define("EFFECTS", serialize($effects));

$colors = array();
for ($i = 0; $i < count($color_names); $i++) {
  $color_index = $i - 2;
  $colors[$color_names[$i]] = $color_index;
  define("COLOR_" . strtoupper($color_names[$i]), $color_index);
}
define("COLORS", serialize($colors));