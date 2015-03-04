#ifndef LEDS_H
#define LEDS_H

typedef enum State {
  OFF = 0, ON = 1
} State;

typedef enum Color {
  UNDEF = -1, NONE = 0, RANDOM = 1, RED = 0b0010, GREEN = 0b0100, BLUE = 0b1000,
  YELLOW = RED | GREEN, MAGENTA = RED | BLUE, CYAN = GREEN | BLUE, WHITE = RED | GREEN | BLUE
} Color;

typedef enum Direction {
  UP = 0, DOWN, LEFT, RIGHT
} Direction;

typedef union Image {
  IMAGE_TYPE image;
  uint8_t bytes[4];
} Image;

#define COLOR_NUMBER   3
#define COLNUM COLOR_NUMBER
#define LEDNUM LED_NUMBER
#define COLS           ROWS
#define LEDS_PER_ROW   (COLS * COLNUM)
#define RED_LED(led)   ((led) * COLNUM + 0)
#define GREEN_LED(led) ((led) * COLNUM + 1)
#define BLUE_LED(led)  ((led) * COLNUM + 2)
#define IS_COLOR(color)          ((color) & RED || (color) & GREEN || (color) & BLUE)
#define COLOR_LED_NUMBER(color)  (IS_COLOR(color) ? RGB_LED_NUMBER : LEDNUM)

class Leds {
  public:
    Leds(Registers& registers, void (*displayHook)());
    bool display(int duration = 0);
    void pointOfView(int pov);
    void append(State value);
    bool get(int led, Color color = NONE);
    void set(int led, State value, Color color = NONE, State colorAddr = ON);
    bool operator[](int led);
    void reset(State append);
    void setAll(State value, Color color = NONE);
    void setRange(int from, int to, State value, Color color = NONE, State colorAddr = ON);
    void toggle(int led, Color color = NONE, State colorAddr = ON);
    Color numberToColor(int number);
    Color randomColor();
    Image getImage(Color color);
    void setImage(Image image, Color color = WHITE);
    void move(Direction direction);
  private:
    Registers& registers;
    Bitmap leds;
    int pov;
    bool doAppend;
    void (*displayHook)();
    byte maps[4][LEDNUM] = {
      { 72, 73, 74, 69, 70, 71, 66, 67,
        68, 63, 64, 65, 60, 61, 62, 45,
        46, 47, 48, 49, 50, 51, 52, 53,
        57, 58, 59, 54, 55, 56, 42, 43,
        44, 39, 40, 41, 33, 36, 37, 38,
        34, 35, 30, 31, 32, 15, 16, 17,
        18, 19, 20, 21, 22, 23, 24, 25,
        26, 27, 28, 29, 12, 13, 14,  9,
        10, 11,  6,  7,  8,  3,  4,  5,
        0,   1,  2
      }
    };
    void dumpState();
    void mapLedsToBits();
    void swapKeyValue(byte* dst, byte* src, size_t num);
    void setupPointOfView(int pov);
};

#endif
