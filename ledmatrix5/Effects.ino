Effects::Effects(Leds& l): leds(l) { }

void Effects::loop() {
  run(&Effects::clock);
  run(&Effects::linearPath);
  run(&Effects::radiantPath);
  run(&Effects::diagonalPath);
}

Effect Effects::find(effectFunc func) {
  for (int i = 0; i < EFFECT_NUMBER; i++)
    if (effects[i].func == func)
      return effects[i];
  return effects[0];
}

void Effects::run(Effect effect, Color color) {
  if (color == UNDEF) color = effect.color;
  leds.reset((State) (effect.options & APPEND));
  bool rotate = effect.options & ROTATE;
  int count = mode == LOOP ? effect.count : 1;
  for (int i = 0; i < count; i++) {
    if (rotate)
      for (int j = 0; j < 4; j++) {
        leds.pointOfView(j % 4);
        (this->*effect.func)(color);
      }
    else
      (this->*effect.func)(color);
    lastEffect = effect;
  }
}

void Effects::run(effectFunc func, Color color) {
  run(find(func), color);
}

void Effects::manual() {
  run(scheduledEffect, scheduledColor);
}

void Effects::schedule(Effect effect, Color color) {
  scheduledEffect = effect;
  scheduledColor = color;
}

void Effects::followPath(byte* path, size_t pathSize, Color color, int duration) {
  byte ledArray[LEDNUM], ledNumber = pathSize * COLNUM;
  for (int i = 0; i < ledNumber; i++)
    ledArray[i] = path[i / COLNUM] * COLNUM + i % COLNUM;
  for (int i = 0; i < ledNumber; i++) {
    leds.setAll(OFF);
    for (int j = 0; j < CYCLE_SPLIT; j++)
      leds.set(ledArray[LO_BOUND(i - j)], ON, color, OFF);
    leds.set(ledArray[i], ON, color, OFF);
    for (int j = 0; j < CYCLE_SPLIT; j++)
      leds.set(ledArray[HI_BOUND(i + j, ledNumber)], ON, color, OFF);
    DISPLAY(duration);
  }
}

void Effects::copyImageParts(Image& dst, Image& src, byte* from, byte* to, int partsSize) {
  for (int i = 0; i < partsSize; i++) {
    int fromBit = RGB_LED_NUMBER - 1 - from[i];
    int toBit   = RGB_LED_NUMBER - 1 - to[i];
    BITMAP_SET(dst.bytes, toBit, BITMAP_GET(src.bytes, fromBit));
  }
}

void Effects::slideToImage(Image image, Color color, Direction direction, int duration, bool spacing) {
  byte to[][ROWS] = {
    { 20, 21, 22, 23, 24 },
    { 0, 1, 2, 3, 4 },
    { 4, 9, 14, 19, 24 },
    { 0, 5, 10, 15, 20 }
  };
  if (spacing) {
    leds.move(direction);
    DISPLAY(duration);
  }
  for (int i = 0; i < ROWS; i++) {
    byte from[ROWS];
    if (direction == UP)
      for (int j = 0; j < ROWS; j++)
        from[j] = j + i * ROWS;
    else if (direction == DOWN)
      for (int j = 0; j < ROWS; j++)
        from[j] = 20 + j - i * ROWS;
    else if (direction == LEFT)
      for (int j = 0; j < ROWS; j++)
        from[j] = j * ROWS + i;
    else if (direction == RIGHT)
      for (int j = 0; j < ROWS; j++)
        from[j] = (j + 1) * ROWS - 1 - i;
    leds.move(direction);
    Image slideImage = leds.getImage(color);
    copyImageParts(slideImage, image, from, to[direction], ROWS);
    leds.setImage(slideImage, color);
    DISPLAY(duration);
  }
}

Image Effects::letterImage(char c) {
  Image letters[] = {
    0b0010001010100011111110001,
    0b1111010001111101000111110,
    0b0111110000100001000001111,
    0b1111010001100011000111110,
    0b1111110000111001000011111,
    0b1111110000111001000010000,
    0b0111110000100111000101111,
    0b1000110001111111000110001,
    0b1111100100001000010011111,
    0b1111100010000101001001100,
    0b1000110010111001001010001,
    0b1000010000100001000011111,
    0b1000111011101011000110001,
    0b1000111001101011001110001,
    0b0111010001100011000101110,
    0b1111010001111101000010000,
    0b0111010001101011001001101,
    0b1111010001111101000110001,
    0b0111110000011100000111110,
    0b1111100100001000010000100,
    0b1000110001100011000101110,
    0b1000110001100010101000100,
    0b1000110001101011101110001,
    0b1000101010001000101010001,
    0b1000101010001000010000100,
    0b1111100010001000100011111,

    0b0111010011101011100101110,
    0b0110000100001000010001110,
    0b1111000001011101000011111,
    0b1111000001001100000111110,
    0b0011001010100101111100010,
    0b1111110000111100000111110,
    0b0111110000111101000101110,
    0b1111100001000100010001000,
    0b0111010001011101000101110,
    0b0111010001011110000111110
  };
  int idx;
  if (c >= 'A' && c <= 'Z')
    idx = c - 'A';
  else if (c >= '0' && c <= '9')
    idx = c - '0' + 26;
  else
    return { 0 };
  return letters[idx];
}

void Effects::slideText(String text, Color color, Direction direction, int duration) {
  int length = text.length();
  if (length == 0) return;
  text.toUpperCase();
  leds.setImage(letterImage(text.charAt(0)), color);
  DISPLAY(duration * 2);
  for (int i = 1; i < length; i++)
    slideToImage(letterImage(text.charAt(i)), color, direction, duration, true);
  DISPLAY(duration * 2);
}

Time Effects::getTime() {
  Process time;
  String timeString;
  time.runShellCommand("date +%H%M");
  while (time.available()) {
    char c = time.read();
    timeString += c;
  }
  byte hours   = timeString.substring(0, 2).toInt();
  byte minutes = timeString.substring(2, 4).toInt();
  return { hours, minutes };
}

void Effects::showClock(Time time, Color color, int duration) {
  byte hoursMap[] = { 7, 8, 8, 13, 18, 18, 17, 16, 16, 11, 6, 6 };
  byte innerMinutesMap[] = { 7, 8, 13, 18, 17, 16, 11, 6, 7 };
  byte outerMinutesMap[] = { 2, 3, 4, 9, 14, 19, 24, 23, 22, 21, 20, 15, 10, 5, 0, 1, 2 };
  leds.setAll(OFF);
  leds.set(12, ON, color);
  leds.set(innerMinutesMap[(int) ((time.minutes + 3.75) / 7.5)], ON, color);
  leds.set(outerMinutesMap[(int) ((time.minutes + 1.875) / 3.75)], ON, color);
  leds.set(hoursMap[time.hours % 12], ON, color);
  DISPLAY(duration);
}
