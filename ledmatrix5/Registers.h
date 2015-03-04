#ifndef REGISTERS_H
#define REGISTERS_H

/*
  Registers
  This class is responsible for accessing the actual matrix hardware,
  being shift registers of type 74HC595.
*/

class Registers {
    friend class Leds;
  public:
    Registers(byte clockPin, byte latchPin, byte dataPin);
    void setup();
    bool get(int bit);
    bool operator[](int bit);
  private:
    byte clockPin, latchPin, dataPin;
    Bitmap bits; // holds LED data
    void shift(); // shifts out actual data
    void set(int bit, bool value);
};

#endif
