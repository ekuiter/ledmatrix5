#ifndef BITMAP_H
#define BITMAP_H

#include "main.h"
#include <stdint.h>

// Bitmap helpers, DO NOT change these
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

/*
  Bitmap
  This class stores a bitmap. Its size is static.
*/

class Bitmap {
  public:
    uint8_t bits[REGISTER_NUMBER]; // bitmap data (as many bytes as there are registers)
    bool get(int bit);
    void set(int bit, bool value);
    // enables reading array access, e.g. bitmap[...] (no writing!)
    bool operator[](int bit);
};

#endif
