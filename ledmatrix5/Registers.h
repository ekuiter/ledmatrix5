#ifndef REGISTERS_H
#define REGISTERS_H

class Registers {
    friend class Leds;
  public:
    Registers(byte clockPin, byte latchPin, byte dataPin);
    void setup();
    bool get(int bit);
    bool operator[](int bit);
  private:
    byte clockPin, latchPin, dataPin;
    Bitmap bits;
    void shift();
    void set(int bit, bool value);
};

#endif
