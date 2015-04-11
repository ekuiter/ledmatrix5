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

$effects = array();
for ($i = 0; $i < count($effect_names); $i++) {
  $effect_index = $i == 0 ? -1 : $i;
  $effects[$effect_names[$i]] = $effect_index;
  define("EFFECT_" . strtoupper($effect_names[$i]), $effect_index);
}

define("COLOR_RANDOM", -2);
define("COLOR_NONE",   -1);
define("COLOR_UNDEF",   0);
define("COLOR_RED",     1);
define("COLOR_GREEN",   2);
define("COLOR_BLUE",    3);
define("COLOR_YELLOW",  4);
define("COLOR_MAGENTA", 5);
define("COLOR_CYAN",    6);
define("COLOR_WHITE",   7);