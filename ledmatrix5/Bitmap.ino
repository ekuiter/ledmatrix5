bool Bitmap::get(int bit) {
  return BITMAP_GET(bits, bit);
}

void Bitmap::set(int bit, bool value) {
  BITMAP_SET(bits, bit, value);
}

bool Bitmap::operator[](int bit) {
  return get(bit);
}
