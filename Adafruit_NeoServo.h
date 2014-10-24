/*------------------------------------------------------------------------
  Adafruit_NeoServo library: uses Timer/Counter 1 (or 3,4,5) to allow use
  of NeoPixels and servos in the same project (with lots of caveats -- see
  the examples for further explanation).

  Written by Phil Burgess for Adafruit Industries, incorporating work
  by Paul Stoffregen, Jesse Tane, Jérôme Despatis, Michael Polli and
  Dan Clemens. The file 'known_16bit_timers.h' is taken verbatim from
  Paul Stoffregen's fork of the TimerOne library:
  https://github.com/PaulStoffregen/TimerOne
  and this code is provided under the same "CC BY 3.0 US" license terms.

  Open Source License

  Adafruit_NeoServo is free software. You can redistribute it and/or
  modify it under the terms of Creative Commons Attribution 3.0 United
  States License. To view a copy of this license, visit
  http://creativecommons.org/licenses/by/3.0/us/
  ------------------------------------------------------------------------*/

#ifndef _ADAFRUIT_NEOSERVO_H_
#define _ADAFRUIT_NEOSERVO_H_

#include <Arduino.h>
#include "known_16bit_timers.h"

#ifdef TIMER1_A_PIN // defined in known_16bit_timers.h
 typedef uint16_t servoPos_t;
#else               // No 16-bit timers on the ATtiny
 #define TIMER1_A_PIN 1
 #define TIMER1_B_PIN 4
 typedef uint8_t servoPos_t;
#endif

class Adafruit_NeoServo {
 public:
  Adafruit_NeoServo(void) : pin(-1), active(false) { };
  void begin(const int8_t p),
       move(const servoPos_t pos),
       stop(void);
  inline servoPos_t read(void) __attribute__((always_inline)) { return *ocr; }

 private:
  int8_t               pin;          // Arduino pin #, or -1 if invalid
  volatile servoPos_t *ocr;          // Output compare A or B reg. for pin
#ifdef TIMER3_A_PIN                  // For chips with >1 timer/counter...
  volatile servoPos_t *counter;      // Timer/Counter count reg. for pin
#endif                               // (else always TCNT1)
  boolean              active;       // True after first move() call
  void                 toggle(void); // PWM on/off
};

#endif // _ADAFRUIT_NEOSERVO_H_
