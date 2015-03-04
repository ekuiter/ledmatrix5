/*
  5x5 LED MATRIX
  =========================================
  © 2015 Elias Kuiter ( http://elias-kuiter.de )
*/

byte clockPin = 9;
byte latchPin = 10;
byte dataPin = 11;

#define REGISTER_NUMBER   10
#define LED_NUMBER        75
#define RGB_LED_NUMBER    25
#define ROWS               5
#define AVG_DUMP_DURATION 25
#define AVG_HOOK_DURATION 10
#define SEED_PIN          A0
#define IMAGE_TYPE        uint32_t
#define TEXT_BUF_SIZE     32

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
    char buf[TEXT_BUF_SIZE];
    Bridge.get("text", buf, TEXT_BUF_SIZE);
    if (effects.currentText != String(buf))
      mode = SWITCHING_MANUAL;
  }
}
