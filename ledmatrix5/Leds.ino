Leds::Leds(Registers& r, void (*d)()): registers(r), displayHook(d) {
  randomSeed(analogRead(SEED_PIN));
  for (int i = 1; i < 4; i++)
    setupPointOfView(i);
}

// a bool is returned to determine whether the current effect should be cancelled (true) or not (false)
bool Leds::display(int duration) {
  unsigned long waitUntil = millis() + duration;
  displayHook();
  if (mode == SWITCHING_LOOP || mode == SWITCHING_MANUAL)
    return true;
  mapLedsToBits();
  registers.shift();
  // When the matrix is displayed for <duration> milliseconds, the processor
  // needs to wait. This wait duration can be used instead of wasted:
  while (millis() < waitUntil) {
    if ((long) (waitUntil - millis()) > AVG_DUMP_DURATION)
      dumpState(); // if there is enough time, tell Linino about the current matrix
    if ((long) (waitUntil - millis()) > AVG_HOOK_DURATION)
      displayHook(); // if there is enough time, ask Linino about new commands
    if (mode == SWITCHING_LOOP || mode == SWITCHING_MANUAL)
      return true; // if Linino sent a new command, do not wait and unwind the stack
  }
  return false;
}

void Leds::dumpState() {
  String str;
  for (int i = REGISTER_NUMBER - 1; i >= 0; i--) {
    String hex = String(leds.bits[i], HEX);
    if (hex.length() == 1)
      str += "0" + hex;
    else
      str += hex;
  }
  Mailbox.writeMessage(str);
}

void Leds::mapLedsToBits() {
  for (int i = 0; i < LEDNUM; i++)
    registers.bits.set(i, leds[maps[pov][i]]);
}

void Leds::pointOfView(int pov) {
  this->pov = pov;
}

void Leds::append(State value) {
  this->doAppend = value;
}

void Leds::swapKeyValue(byte* dst, byte* src, size_t num) {
  for (int i = 0; i < num; i++)
    dst[src[i]] = i;
}

void Leds::setupPointOfView(int pov) {
  byte srcMap[LEDNUM], dstMap[LEDNUM], transposeMap[LEDNUM] =
  { 72, 73, 74, 57, 58, 59, 42, 43, // transposeMap does not need to be adjusted (whatever maps[...] is).
    44, 27, 28, 29, 12, 13, 14,  9, // It describes how a 90 degrees rotation looks like.
    10, 11, 24, 25, 26, 39, 40, 41,
    69, 70, 71, 54, 55, 56, 66, 67,
    68, 51, 52, 53, 21, 36, 37, 38,
    22, 23,  6,  7,  8,  3,  4,  5,
    18, 19, 20, 33, 34, 35, 48, 49,
    50, 63, 64, 65, 60, 61, 62, 45,
    46, 47, 30, 31, 32, 15, 16, 17,
    0,   1,  2
  };
  if (pov == 1) {
    swapKeyValue(srcMap, transposeMap, LEDNUM);
    for (int i = 0; i < LEDNUM; i += LEDS_PER_ROW)
      memcpy(&dstMap[LEDNUM - LEDS_PER_ROW - i], &srcMap[i], LEDS_PER_ROW);
  }
  if (pov == 2) {
    swapKeyValue(srcMap, maps[0], LEDNUM);
    for (int i = 0; i < LEDNUM; i += COLNUM)
      memcpy(&dstMap[i], &srcMap[LEDNUM - COLNUM - i], COLNUM);
  }
  if (pov == 3) {
    swapKeyValue(srcMap, transposeMap, LEDNUM);
    for (int i = 0; i < LEDNUM; i += COLNUM)
      memcpy(&dstMap[i], &srcMap[i / LEDS_PER_ROW * LEDS_PER_ROW + LEDS_PER_ROW - COLNUM - i % LEDS_PER_ROW], COLNUM);
  }
  swapKeyValue(maps[pov], dstMap, LEDNUM);
}

bool Leds::get(int led, Color color) {
  if (color & RED)      return leds[RED_LED(led)];
  if (color & GREEN)    return leds[GREEN_LED(led)];
  if (color & BLUE)     return leds[BLUE_LED(led)];
  if (!IS_COLOR(color)) return leds[led];
}

void Leds::set(int led, State value, Color color, State colorAddr) {
  if (color & RANDOM)
    color = randomColor();
  int ledNumber = colorAddr ? COLOR_LED_NUMBER(color) : LEDNUM;
  if (doAppend) {
    if (led < 0)          led += ledNumber;
    if (led >= ledNumber) led -= ledNumber;
  } else {
    if (led < 0)          return;
    if (led >= ledNumber) return;
  }
  led = IS_COLOR(color) && !colorAddr ? led / 3 : led;
  if (color & RED)      leds.set(RED_LED(led),   value);
  if (color & GREEN)    leds.set(GREEN_LED(led), value);
  if (color & BLUE)     leds.set(BLUE_LED(led),  value);
  if (!IS_COLOR(color)) leds.set(led,            value);
}

bool Leds::operator[](int led) {
  return get(led);
}

void Leds::reset(State doAppend) {
  pointOfView(0);
  setAll(OFF);
  append(doAppend);
}

void Leds::setAll(State value, Color color) {
  int ledNumber = COLOR_LED_NUMBER(color);
  for (int i = 0; i < ledNumber; i++)
    set(i, value, color);
}

void Leds::setRange(int from, int to, State value, Color color, State colorAddr) {
  for (int i = from; i <= to; i++)
    set(i, value, color, colorAddr);
}

void Leds::toggle(int led, Color color, State colorAddr) {
  set(led, (State) !get(led, color), color);
}

Color Leds::numberToColor(int number) {
  if (number == -2) return RANDOM;
  if (number == -1) return NONE;
  if (number == 0)  return UNDEF;
  if (number == 1)  return RED;
  if (number == 2)  return GREEN;
  if (number == 3)  return BLUE;
  if (number == 4)  return YELLOW;
  if (number == 5)  return MAGENTA;
  if (number == 6)  return CYAN;
  if (number == 7)  return WHITE;
  return UNDEF;
}

Color Leds::randomColor() {
  return numberToColor(random(1, 8));
}

Image Leds::getImage(Color color) {
  Image image;
  for (int i = 0; i < RGB_LED_NUMBER; i++)
    BITMAP_SET(image.bytes, i, get(RGB_LED_NUMBER - 1 - i, color));
  return image;
}

void Leds::setImage(Image image, Color color) {
  for (int i = 0; i < COLOR_LED_NUMBER(color); i++)
    set(COLOR_LED_NUMBER(color) - 1 - i, (State) (image.bytes[INDEX(i)] & SHIFT(i)), color);
}

void Leds::move(Direction direction) {
  if (direction == UP)
    for (int i = 0; i < LEDNUM; i++)
      if (i < LEDNUM - LEDS_PER_ROW)
        leds.set(i, leds[i + LEDS_PER_ROW]);
      else
        leds.set(i, OFF);
  else if (direction == DOWN)
    for (int i = LEDNUM - 1; i >= 0; i--)
      if (i >= LEDS_PER_ROW)
        leds.set(i, leds[i - LEDS_PER_ROW]);
      else
        leds.set(i, OFF);
  else if (direction == LEFT)
    for (int i = 0; i < LEDNUM; i++)
      if (i % LEDS_PER_ROW < LEDS_PER_ROW - COLNUM)
        leds.set(i, leds[i + COLNUM]);
      else
        leds.set(i, OFF);
  else if (direction == RIGHT)
    for (int i = LEDNUM - 1; i >= 0; i--)
      if (i % LEDS_PER_ROW >= COLNUM)
        leds.set(i, leds[i - COLNUM]);
      else
        leds.set(i, OFF);
}
