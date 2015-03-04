#ifndef LEDS_H
#define LEDS_H

// DO NOT change these definitions
#define COLOR_NUMBER   3                    // the colors RED,GREEN,BLUE
#define COLNUM COLOR_NUMBER                 // abbreviations
#define LEDNUM LED_NUMBER
#define COLS           ROWS                 // a square matrix has as many columns as rows
#define LEDS_PER_ROW   (COLS * COLNUM)
#define RED_LED(led)   ((led) * COLNUM + 0) // turns a RED,
#define GREEN_LED(led) ((led) * COLNUM + 1) //       a GREEN,
#define BLUE_LED(led)  ((led) * COLNUM + 2) //       a BLUE  COLOR address into a NONE address
// determines whether COLOR or NONE addressing should be used
#define IS_COLOR(color)          ((color) & RED || (color) & GREEN || (color) & BLUE)
// determines the number of LEDs for COLOR or NONE addressing
#define COLOR_LED_NUMBER(color)  (IS_COLOR(color) ? RGB_LED_NUMBER : LEDNUM)

// a "bool-ish" type to define LED and other states
typedef enum State {
  OFF = 0, ON = 1
} State;

// Defines all possible colors.
// UNDEF means "use the default color"
// NONE means "ignore color and use NONE addressing"
// RANDOM means "use a random color"
// RED, GREEN, BLUE are authentic colors whereas
// YELLOW, MAGENTA, CYAN, WHITE are mixed
typedef enum Color {
  UNDEF = -1, NONE = 0, RANDOM = 1, RED = 0b0010, GREEN = 0b0100, BLUE = 0b1000,
  YELLOW = RED | GREEN, MAGENTA = RED | BLUE, CYAN = GREEN | BLUE, WHITE = RED | GREEN | BLUE
} Color;
// It is important to understand COLOR and NONE addressing.
// COLOR addressing means:
//   Every RGB LED (combination of 3 single LEDs) has ONE address.
//   That means there are 0-24 RED LEDs, 0-24 GREEN LEDs and so on.
//   This COLOR address is used below in get(...), set(...) and other functions.
// NONE addressing means:
//   Every single LED has ONE address, ignoring the RGB-combined LEDs.
//   That means there are 0-74 NONE LEDs. 0,1,2 is the first RGB LED, 2,3,4 the 2nd and so on.
//   The colors RED,GREEN,BLUE etc. are NOT distinguished.
//   You can enable NONE addressing in get(...), set(...) and other functions below
//   by using the Color NONE or by setting colorAddr to OFF (explained in detail below).

// constants for matrix movement (see move(...) below)
typedef enum Direction {
  UP = 0, DOWN, LEFT, RIGHT
} Direction;

// Images can be displayed in the matrix using setImage(...) below. They can also represent
// the current matrix, see getImage(...) below.
// An image is a number of 32 bits, the bits 0-24 each represent one LED's state (ON or OFF),
// therefore an image has only one color.
typedef union Image {
  uint32_t image;
  uint8_t bytes[4];
} Image;

/*
  Leds
  This class deals with the mathematics of getting the matrix to work.
  It provides an API for turning LEDs on / off, setting colors and more.
*/

class Leds {
  public:
    // displayHook is called whenever there is enough time until the next display(...) call
    Leds(Registers& registers, void (*displayHook)());
    bool display(int duration = 0); // displays the current set of data for <duration> milliseconds
    // rotates the matrix by
    // pov = 0 => 0   degrees, pov = 1 => 90  degrees,
    // pov = 2 => 180 degrees, pov = 3 => 270 degrees
    void pointOfView(int pov);
    // set whether LEDs > LED_NUMBER should start again at the top
    // and whether LEDs < 0 should start again at the bottom of the matrix, can be ON or OFF
    void append(State value);
    // returns whether an led is ON or OFF for the given color, distinguishes between COLOR and NONE addressing!
    bool get(int led, Color color = NONE);
    // sets whether an led is ON or OFF for the given color
    // if a color is given: uses COLOR addressing by default, if colorAddr = OFF, NONE addressing is used
    // if color = NONE, NONE addressing is used (no matter what colorAddr is)
    void set(int led, State value, Color color = NONE, State colorAddr = ON);
    bool operator[](int led);
    void reset(State doAppend); // resets the matrix (default pov, all LEDs OFF, set append(...) as given)
    void setAll(State value, Color color = NONE); // sets whether all leds are ON or OFF for the given color
    // sets all LEDs in a range to a given value and color, beware addressing as in set(...)!
    void setRange(int from, int to, State value, Color color = NONE, State colorAddr = ON);
    void toggle(int led, Color color = NONE, State colorAddr = ON); // toggles an LED' state, see set(...)
    Color numberToColor(int number);
    Color randomColor();
    Image getImage(Color color); // gets the current matrix as an image
    void setImage(Image image, Color color = WHITE); // sets the current matrix to an image with the given color
    // moves the current matrix one column / row in the given direction, the column / row that "leaves" the
    // matrix is removed from memory, the new column / row is filled with LEDs that are OFF
    void move(Direction direction);
  private:
    Registers& registers; // holds the hardware layer (and the actual data)
    Bitmap leds; // holds the LED data (the virtual data)
    int pov; // point of view, sets the matrix rotation as described at pointOfView(...) above
    bool doAppend; // whether LEDs should prepend or append, see append(...) above
    void (*displayHook)(); // see constructor Leds(...) above
    byte maps[4][LEDNUM] = { // maps virtual LEDs to the real hardware
      { 72, 73, 74, 69, 70, 71, 66, 67, // the array to the left represents the visual structure of the matrix
        68, 63, 64, 65, 60, 61, 62, 45, // 72,73,74 is the top left LED with the virtual COLOR address 0 and
        46, 47, 48, 49, 50, 51, 52, 53, // the NONE addresses 0,1,2.
        57, 58, 59, 54, 55, 56, 42, 43, // The numbers in this matrix are the bit addresses for the shift registers.
        44, 39, 40, 41, 33, 36, 37, 38, // In my case, the bottom right LED is the first addressed by the registers,
        34, 35, 30, 31, 32, 15, 16, 17, // so it has the numbers 0,1,2 (in the order RED,GREEN,BLUE).
        18, 19, 20, 21, 22, 23, 24, 25, // An ideal map where a virtual address is identical to its real address
        26, 27, 28, 29, 12, 13, 14,  9, // would look like this: { 0, 1, 2, 3, 4, ..., 70, 71, 72, 73, 74 }
        10, 11,  6,  7,  8,  3,  4,  5, // Because of the practical construction of the matrix this ideal map
        0,   1,  2                      // is not easy to build on the hardware side, so I used the map on the left.
      }
    };
    void dumpState(); // notifies Linino about the current matrix, similar to displayHook() above
    void mapLedsToBits();
    void swapKeyValue(byte* dst, byte* src, size_t num);
    void setupPointOfView(int pov);
};

#endif
