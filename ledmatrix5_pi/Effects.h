#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdint.h>
#include <string>
#include <vector>
#include "Leds.h"

// DO NOT change these definitions
#define ROTATE      0b01 // whether an effect should rotate
#define APPEND      0b10 // whether an effect should use appending
#define CYCLE_RANGE 9    // how many NONE LEDs should be turned on by followPath
#define CYCLE_SPLIT (CYCLE_RANGE / 2)
#define LO_BOUND(expr)    ((expr) < 0     ? 0     : (expr)) // avoid buffer overflows
#define HI_BOUND(expr, h) ((expr) > h - 1 ? h - 1 : (expr))

class Effects;

// effect function pointer type
typedef void (Effects::*effectFunc)(Color color);
typedef void (Effects::*effectFuncWithArg)(Color color, const std::string& arg);

typedef struct Effect { // an effect consists of ...
  std::string name;     // a name
  effectFunc func;      // an effect function pointer
  effectFuncWithArg funcWithArg;
  Color      color;     // default effect color
  uint8_t    options;   // options like ROTATE or APPEND
} Effect;

typedef struct EffectInvocation {
  Effect effect;
  Color color;
  std::string arg;
} EffectInvocation;

typedef struct Time {
  uint8_t hours, minutes;
} Time;

/*
  Effects
  This class contains the actual effects as well as helper functions.
*/

class Effects {
  public:
    // The effect map. effect[0] is a fallback, see find(...)
    std::vector<Effect> effects {
      { "idle",         &Effects::idle,         nullptr,        NONE,  NONE },
      { "loop",         &Effects::loop,         nullptr,        UNDEF, NONE },
      { "light",        &Effects::light,        nullptr,        WHITE, NONE },
      { "linearPath",   &Effects::linearPath,   nullptr,        NONE,  ROTATE },
      { "radiantPath",  &Effects::radiantPath,  nullptr,        NONE,  ROTATE },
      { "diagonalPath", &Effects::diagonalPath, nullptr,        NONE,  ROTATE },
      { "text",         nullptr,                &Effects::text, WHITE, NONE },
      { "clock",        &Effects::clock,        nullptr,        WHITE, NONE }
    };
    Leds& leds;
    Effects(Leds& leds);
    // returns the effect for an effect function pointer
    Effect find(const std::string& name);
    Effect find(effectFunc func);
    Effect find(effectFuncWithArg funcWithArg);
    void invoke(const EffectInvocation& effectInvocation);
    void run(EffectInvocation effectInvocation); // runs a given effect
    void run(Effect effect, Color color = UNDEF, const std::string& arg = "");
    void run(effectFunc func, Color color = UNDEF);
    void run(effectFuncWithArg funcWithArg, Color color = UNDEF, const std::string& arg = "");
    void run(); // runs the currently scheduledEffect
    void schedule(const EffectInvocation& effectInvocation); // schedules a new effect
    // the actual effect functions defined in EffectFuncs.ino
    void loop(Color color);
    void idle(Color color);
    void light(Color color);
    void linearPath(Color color);
    void radiantPath(Color color);
    void diagonalPath(Color color);
    void text(Color color, const std::string& arg);
    void clock(Color color);
  private:
    EffectInvocation scheduledEffectInvocation { .effect = find(&Effects::loop), .color = UNDEF, .arg = "" };
    Effect lastEffect = find(&Effects::loop);
    // creates an LED path, e.g. path = {0,1,,....,23,24}, pathSize = 25
    void followPath(const std::vector<uint8_t>& path, Color color, int duration);
    // copies src's LED states of the LEDs in "from" to dst's LED states of the LEDs in "to",
    // e.g. from = {0,1,2,3,4}, to = {20,21,22,23,24}, partsSize = 5 would copy src's first to dst's last row
    void copyImageParts(Image& dst, Image& src, uint8_t* from, uint8_t* to, int partsSize);
    // slides the matrix to a new image in a given direction, duration is the duration for each slide frame,
    // if spacing = true an additional column / row is inserted
    void slideToImage(Image image, Color color, Direction direction, int duration, bool spacing = false, int omittedRows = 0);
    Image letterImage(char c); // returns an image for the letters A-Z and digits 0-9
    // slides a text, similar to slideToImage(...)
    void slideText(std::string text, Color color = WHITE, Direction direction = LEFT, int duration = 120);
    Time getTime();
    void showClock(Time time, Color color, int duration); // draws a clock for the given time
};

#endif
