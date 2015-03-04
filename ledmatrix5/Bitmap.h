#ifndef BITMAP_H
#define BITMAP_H

#define INDEX(bit)  ((bit) / 8)
#define OFFSET(bit) ((bit) % 8)
#define SHIFT(bit)  (1 << OFFSET(bit))
#define BITMAP_GET(bitmap, bit) \
  ((bitmap)[INDEX(bit)] & SHIFT(bit))
#define BITMAP_SET(bitmap, bit, value) \
  if (value) \
    (bitmap)[INDEX(bit)] |= SHIFT(bit); \
  else \
    (bitmap)[INDEX(bit)] &= ~(SHIFT(bit))

class Bitmap {
  public:
    byte bits[REGISTER_NUMBER];
    bool get(int bit);
    void set(int bit, bool value);
    bool operator[](int bit);
};

#endif
