#ifndef EFFECTS_H
#define EFFECTS_H

#define EFFECT_NUMBER 7

#define ROTATE      0b01
#define APPEND      0b10
#define CYCLE_RANGE 9
#define CYCLE_SPLIT (CYCLE_RANGE / 2)
#define LO_BOUND(expr)    ((expr) < 0     ? 0     : (expr))
#define HI_BOUND(expr, h) ((expr) > h - 1 ? h - 1 : (expr))
#define DISPLAY(duration) if(leds.display(duration)) return

class Effects;

typedef void (Effects::*effectFunc)(Color color);

typedef struct Effect {
  effectFunc func;
  Color      color;
  byte       options;
  byte       count;
} Effect;

typedef struct Time {
  byte hours;
  byte minutes;
} Time;

class Effects {
  public:
    Effect effects[EFFECT_NUMBER] = {
      { 0, NONE, NONE, 0 },
      { &Effects::idle, NONE, NONE, 1 },
      { &Effects::linearPath, NONE, ROTATE, 1 },
      { &Effects::radiantPath, NONE, ROTATE, 1 },
      { &Effects::diagonalPath, NONE, ROTATE, 1 },
      { &Effects::text, WHITE, NONE, 1 },
      { &Effects::clock, WHITE, NONE, 1 }
    };
    Leds& leds;
    Effects(Leds& leds);
    Effect scheduledEffect;
    String currentText;
    void loop();
    Effect find(effectFunc func);
    void run(Effect effect, Color color = UNDEF);
    void run(effectFunc func, Color color = UNDEF);
    void manual();
    void schedule(Effect effect, Color color);
    void idle(Color color);
    void linearPath(Color color);
    void radiantPath(Color color);
    void diagonalPath(Color color);
    void text(Color color);
    void clock(Color color);
  private:
    Effect lastEffect;
    Color scheduledColor;
    void followPath(byte* path, size_t pathSize, Color color, int duration);
    void copyImageParts(Image& dst, Image& src, byte* from, byte* to, int partsSize);
    void slideToImage(Image image, Color color, Direction direction, int duration, bool spacing = false);
    Image letterImage(char c);
    void slideText(String text, Color color = WHITE, Direction direction = LEFT, int duration = 150);
    Time getTime();
    void showClock(Time time, Color color, int duration);
};

#endif
