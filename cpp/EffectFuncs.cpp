#include "Effects.h"

void Effects::loop(Color color) {
  run(&Effects::clock, color);
  run(&Effects::linearPath, color);
  run(&Effects::radiantPath, color);
  run(&Effects::diagonalPath, color);
}

void Effects::idle(Color color) {
  leds.display(100);
}

void Effects::light(Color color) {
  leds.setAll(ON, color);
  leds.display(100);
}

void Effects::linearPath(Color color) {
  for (int i = 0; i < LEDNUM; i++) {
    leds.setAll(OFF);
    leds.setRange(i - CYCLE_SPLIT, i + CYCLE_SPLIT, ON, color, OFF);
    leds.display(50);
  }
}

void Effects::radiantPath(Color color) {
  followPath({ 0, 1, 2, 3, 4, 9, 14, 19, 24, 23, 22, 21, 20, 15, 10, 5, 6, 7, 8, 13, 18, 17, 16, 11, 12 }, color, 15);
  followPath({ 11, 16, 17, 18, 13, 8, 7, 6, 5, 10, 15, 20, 21, 22, 23, 24, 19, 14, 9 }, color, 15);
}

void Effects::diagonalPath(Color color) {
  followPath({ 0, 5, 1, 10, 6, 2, 15, 11, 7, 3, 20, 16, 12, 8, 4, 21, 17, 13, 9, 22, 18, 14, 23, 19, 24 }, color, 30);
}

void Effects::text(Color color, const std::string& arg) {
  if (arg.length())
    slideText(arg, color);
  else
    run(&Effects::idle);
}

void Effects::clock(Color color) {
  Time time = getTime(), incTime = { 0, 0 };
  if (lastEffect.func != &Effects::clock) {
    for (int j = 0; j < time.minutes; j++) {
      int speed = j * 50 / time.minutes;
      showClock(incTime, color, speed);
      incTime.hours = j / (time.minutes / (float) (time.hours % 12));
      incTime.minutes++;
    }
  }
  showClock(time, color, 2000);
}
