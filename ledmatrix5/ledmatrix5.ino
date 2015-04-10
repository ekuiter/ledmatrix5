/*
  5x5 RGB LED MATRIX + Effects + Linino
  =====================================
  © 2015 Elias Kuiter ( http://elias-kuiter.de )
*/

// This sketch is to be used with a shift-register-powered RGB LED matrix.
// The 74HC595's data pins can be set here:
byte clockPin = 9;
byte latchPin = 10;
byte dataPin = 11;

// If you want to use this sketch yourself, remember to change the LED maps
// in Leds.h according to you matrix hardware!!

// These constants tell the board how long it should take to talk to Linino.
// The default values should work out just fine. Change them if there are matrix "lags".
#define AVG_DUMP_DURATION 25 // The board notifies Linino about the current matrix.
#define AVG_HOOK_DURATION 10 // The board receives new commands from Linino.

// A "random" seed is grabbed from this analog pin. Note that this
// is not a perfect, but an easy way to create randomness.
#define SEED_PIN          A0

// Determines how long the text displayed by the text effect may be.
#define TEXT_MAX_LENGTH   60

// These constants can be altered, but the effects are made for a square
// 5 by 5 matrix. If you change them, the effects might not work as
// expected or the matrix might stop working at all.
#define REGISTER_NUMBER   10 // number of shift registers
#define LED_NUMBER        75 // number of single LEDs (NONE  addressing)
#define RGB_LED_NUMBER    25 // number of RGB LEDs    (COLOR addressing)
#define ROWS               5 // number of rows in the matrix

#include <Console.h>
#include <Mailbox.h>
#include "Bitmap.h"
#include "Registers.h"
#include "Leds.h"
#include "Effects.h"
#include "Connection.h"

Registers registers(clockPin, latchPin, dataPin);
Leds leds(registers, &displayHook);
Effects effects(leds);
Connection connection(effects);

// Defines all possible modes.
// LOOP is the default mode when the board is powered on (see effects.loop())
// MANUAL is used to run Linino commands (see effects.manual()
// SWITCHING_* indicates that Linino wants to switch modes, running effects
// are cancelled so that the switch can happen ASAP
enum mode {
  LOOP, MANUAL, SWITCHING_LOOP, SWITCHING_MANUAL
} mode = LOOP;

void setup() {
  connection.setup();
  registers.setup();
}

void loop() {
  if (mode == SWITCHING_LOOP || mode == LOOP) {
    mode = LOOP;
    effects.loop();
  }
  if (mode == SWITCHING_MANUAL || mode == MANUAL) {
    mode = MANUAL;
    effects.manual();
  }
}

void displayHook() {
  connection.receive();
  if (mode == MANUAL && effects.scheduledEffect.func == &Effects::text) {
    // If there is a new text, display it. Because the text effect has a long
    // duration (~150ms per slide frame), we can afford to query the Bridge.
    char buf[TEXT_MAX_LENGTH + 1];
    buf[TEXT_MAX_LENGTH] = 0;
    Bridge.get("text", buf, TEXT_MAX_LENGTH);
    String str = String(buf);
    if (effects.currentText != str) {
      effects.currentText = str;
      effects.currentTextUpdated = true;
      mode = SWITCHING_MANUAL;
    }
  }
}
