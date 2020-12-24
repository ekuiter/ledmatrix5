#ifndef MAIN_H
#define MAIN_H

// This sketch is to be used with a shift-register-powered RGB LED matrix.
// The 74HC595's data pins can be set here:
#define CLOCK_PIN         29
#define LATCH_PIN         28
#define DATA_PIN          27

// If you want to use this sketch yourself, remember to change the LED maps
// in Leds.h according to you matrix hardware!!

// These constants tell the board how long dumping and receiving should take.
// The default values should work out just fine. Change them if there are matrix "lags".
#define AVG_DUMP_DURATION 4 // The board dumps the current matrix state.
#define AVG_RECV_DURATION 4 // The board receives a new command.

// These constants can be altered, but the effects are made for a square
// 5 by 5 matrix. If you change them, the effects might not work as
// expected or the matrix might stop working at all.
#define REGISTER_NUMBER   10 // number of shift registers
#define LED_NUMBER        75 // number of single LEDs (NONE  addressing)
#define RGB_LED_NUMBER    25 // number of RGB LEDs    (COLOR addressing)
#define ROWS               5 // number of rows in the matrix

#endif
