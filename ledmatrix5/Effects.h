#ifndef EFFECTS_H
#define EFFECTS_H

// change this if you add or remove effects
#define EFFECT_NUMBER 8

// DO NOT change these definitions
#define ROTATE      0b01 // whether an effect should rotate
#define APPEND      0b10 // whether an effect should use appending
#define CYCLE_RANGE 9    // how many NONE LEDs should be turned on by followPath
#define CYCLE_SPLIT (CYCLE_RANGE / 2)
#define LO_BOUND(expr)    ((expr) < 0     ? 0     : (expr)) // avoid buffer overflows
#define HI_BOUND(expr, h) ((expr) > h - 1 ? h - 1 : (expr))
// leave an effect if Linino sent a new command, DO NOT use leds.display(...) directly!
#define DISPLAY(duration) if (leds.display(duration)) return

class Effects;

typedef void (Effects::*effectFunc)(Color color, int argument); // effect function pointer type

typedef struct Effect { // an effect consists of ...
  effectFunc func;      // an effect function pointer
  Color      color;     // a color (sent by Linino in MANUAL mode, can be ignored by the effect)
  byte       options;   // options like ROTATE or APPEND
  byte       count;     // how often the effect should be run in LOOP mode
} Effect;

typedef struct Time {
  byte hours;
  byte minutes;
} Time;

/*
  Effects
  This class contains the actual effects as well as helper functions.
  Effects can be run in LOOP (see loop()) or MANUAL (see manual()) mode.
*/

class Effects {
  public:
    // The effect map. The order is important for Linino commands, so do not change it.
    Effect effects[EFFECT_NUMBER] = {
      { 0, NONE, NONE, 0 },
      { &Effects::idle, NONE, NONE, 1 },
      { &Effects::light, WHITE, NONE, 1 },
      { &Effects::linearPath, NONE, ROTATE, 1 },
      { &Effects::radiantPath, NONE, ROTATE, 1 },
      { &Effects::diagonalPath, NONE, ROTATE, 1 },
      { &Effects::text, WHITE, NONE, 1 },
      { &Effects::clock, WHITE, NONE, 1 }
    };
    Leds& leds;
    Effects(Leds& leds);
    Effect scheduledEffect; // holds the effect to be run next in MANUAL mode
    String currentText; // the text to be displayed by the text effect
    bool currentTextUpdated; // is set to true on text updates to avoid multiple calls to Bridge
    void loop(); // the loop which is run in LOOP mode
    Effect find(effectFunc func); // returns the effect for an effect function pointer
    void run(Effect effect, Color color = UNDEF, int argument = -1); // runs a given effect
    void run(effectFunc func, Color color = UNDEF, int argument = -1);
    void manual(); // runs the currently scheduledEffect
    void schedule(Effect effect, Color color, int argument); // schedules a new effect
    // the actual effect functions defined in EffectFuncs.ino
    void idle(Color color, int argument);
    void light(Color color, int argument);
    void linearPath(Color color, int argument);
    void radiantPath(Color color, int argument);
    void diagonalPath(Color color, int argument);
    void text(Color color, int argument);
    void clock(Color color, int argument);
  private:
    Effect lastEffect;
    Color scheduledColor;
    int scheduledArgument;
    // creates an LED path, e.g. path = {0,1,,....,23,24}, pathSize = 25
    void followPath(byte* path, size_t pathSize, Color color, int duration);
    // copies src's LED states of the LEDs in "from" to dst's LED states of the LEDs in "to",
    // e.g. from = {0,1,2,3,4}, to = {20,21,22,23,24}, partsSize = 5 would copy src's first to dst's last row
    void copyImageParts(Image& dst, Image& src, byte* from, byte* to, int partsSize);
    // slides the matrix to a new image in a given direction, duration is the duration for each slide frame,
    // if spacing = true an additional column / row is inserted
    void slideToImage(Image image, Color color, Direction direction, int duration, bool spacing = false, int omittedRows = 0);
    Image letterImage(char c); // returns an image for the letters A-Z and digits 0-9
    // slides a text, similar to slideToImage(...)
    void slideText(String text, Color color = WHITE, Direction direction = LEFT, int duration = 120);
    Time getTime(); // asks Linino for the current time and returns it
    void showClock(Time time, Color color, int duration); // draws a clock for the given time
};

#endif
