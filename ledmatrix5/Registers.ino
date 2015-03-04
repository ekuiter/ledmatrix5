Registers::Registers(byte c, byte l, byte d): clockPin(c), latchPin(l), dataPin(d) { }

void Registers::setup() {
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW);
  digitalWrite(dataPin, LOW);
}

void Registers::shift() {
  digitalWrite(latchPin, LOW);
  for (int i = REGISTER_NUMBER - 1; i >= 0; i--)
    // uses the native Arduino implementation to shift out data
    shiftOut(dataPin, clockPin, MSBFIRST, ~(bits.bits[i]));
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
