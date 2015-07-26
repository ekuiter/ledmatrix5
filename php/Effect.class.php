<?php

class Effect {
  private $effect;

  public function __construct($effect) {
    $this->effect = self::getId($effect);
  }

  public function run($color = COLOR_UNDEF, $argument = -1) {
    $ledMatrix = new LedMatrix(MATRIX_HOST, MATRIX_WRITE);
    $color = self::getColor($color);
    if ($this->effect == EFFECT_TEXT && $argument != -1)
      $ledMatrix->setText($argument);
    $ledMatrix->run($this->effect, $color, intval($argument));
  }

  public static function getId($effect) {
    if (is_numeric($effect) && intval($effect) >= 0 && intval($effect) < count(unserialize(EFFECTS)))
      return intval($effect);
    else if (array_key_exists($effect, unserialize(EFFECTS)))
      return intval(unserialize(EFFECTS)[$effect]);
    else
      throw new InvalidArgumentException("invalid effect");
  }

  public static function getColor($color) {
    if (is_numeric($color) && intval($color) >= -2 && intval($color) < count(unserialize(COLORS)) - 2)
      return intval($color);
    else if (array_key_exists($color, unserialize(COLORS)))
      return intval(unserialize(COLORS)[$color]);
    else
      throw new InvalidArgumentException("invalid color");
  }
}