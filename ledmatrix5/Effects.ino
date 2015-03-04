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
    { 20, 21, 22, 23, 24 }, // UP    (move up and append image's row to the bottom)
    { 0, 1, 2, 3, 4 },      // DOWN  (move down and append image's row to the top)
    { 4, 9, 14, 19, 24 },   // LEFT  (move left and append image's column to the right)
    { 0, 5, 10, 15, 20 }    // RIGHT (move right and append image's column to the left)
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
    // letters A-Z
    0b0010001010100011111110001, // A
    0b1111010001111101000111110, // B
    0b0111110000100001000001111, // C
    0b1111010001100011000111110, // D
    0b1111110000111001000011111, // E
    0b1111110000111001000010000, // F
    0b0111110000100111000101111, // G
    0b1000110001111111000110001, // H
    0b1111100100001000010011111, // I
    0b1111100010000101001001100, // J
    0b1000110010111001001010001, // K
    0b1000010000100001000011111, // L
    0b1000111011101011000110001, // M
    0b1000111001101011001110001, // N
    0b0111010001100011000101110, // O
    0b1111010001111101000010000, // P
    0b0111010001101011001001101, // Q
    0b1111010001111101000110001, // R
    0b0111110000011100000111110, // S
    0b1111100100001000010000100, // T
    0b1000110001100011000101110, // U
    0b1000110001100010101000100, // V
    0b1000110001101011101110001, // W
    0b1000101010001000101010001, // X
    0b1000101010001000010000100, // Y
    0b1111100010001000100011111, // Z
    // digits 0-9
    0b0111010011101011100101110, // 0
    0b0110000100001000010001110, // 1
    0b1111000001011101000011111, // 2
    0b1111000001001100000111110, // 3
    0b0011001010100101111100010, // 4
    0b1111110000111100000111110, // 5
    0b0111110000111101000101110, // 6
    0b1111100001000100010001000, // 7
    0b0111010001011101000101110, // 8
    0b0111010001011110000111110  // 9
  };
  int idx;
  if (c >= 'A' && c <= 'Z')
    idx = c - 'A';
  else if (c >= '0' && c <= '9')
    idx = c - '0' + 26;
  else
    return { 0 }; // empty image
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
  byte hoursMap[] = { 7, 8, 8, 13, 18, 18, 17, 16, 16, 11, 6, 6 }; // LED addresses for each hour
  byte innerMinutesMap[] = { 7, 8, 13, 18, 17, 16, 11, 6, 7 }; // and for the minutes (inner circle)
  byte outerMinutesMap[] = { 2, 3, 4, 9, 14, 19, 24, 23, 22, 21, 20, 15, 10, 5, 0, 1, 2 };
  leds.setAll(OFF);
  leds.set(12, ON, color); // always enable the central LED
  leds.set(innerMinutesMap[(int) ((time.minutes + 3.75) / 7.5)], ON, color); // use rule of three to
  leds.set(outerMinutesMap[(int) ((time.minutes + 1.875) / 3.75)], ON, color); // draw a minute hand
  leds.set(hoursMap[time.hours % 12], ON, color);
  DISPLAY(duration);
}
