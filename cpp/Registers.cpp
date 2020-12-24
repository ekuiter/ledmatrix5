#include "Registers.h"
#include <wiringPi.h>
#include <wiringShift.h>
#include <stdio.h>

Registers::Registers(uint8_t c, uint8_t l, uint8_t d): clockPin(c), latchPin(l), dataPin(d) { }

void Registers::setup() {
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW);
  digitalWrite(dataPin, LOW);
}

// This is needed because the Pi is too fast for the shift registers to keep up ...
void slowShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
	for (uint8_t i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else	
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));
    delayMicroseconds(1);
		digitalWrite(clockPin, HIGH);
    delayMicroseconds(1);
		digitalWrite(clockPin, LOW);		
    delayMicroseconds(1);
	}
}

void Registers::shift() {
  digitalWrite(latchPin, LOW);
  for (int i = REGISTER_NUMBER - 1; i >= 0; i--)
    slowShiftOut(dataPin, clockPin, MSBFIRST, ~(bits.bits[i]));
  digitalWrite(latchPin, HIGH);
}

bool Registers::get(int bit) {
  return bits[bit];
}

void Registers::set(int bit, bool value) {
  bits.set(bit, value);
}

bool Registers::operator[](int bit) {
  return get(bit);
}
