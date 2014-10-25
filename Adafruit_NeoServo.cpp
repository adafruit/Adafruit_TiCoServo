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

#include "Adafruit_NeoServo.h"

void Adafruit_NeoServo::attach(const int8_t p) {

  // Referencing any timer/counter configures it to ~50 Hz and sets all
  // output compare registers to a roughly centered-ish position for
  // servos (1.5 ms).  PWM output for the pin is NOT enabled until the
  // first call to write().

#if defined(__AVR_ATtiny85__)
  // Adafruit Trinket, etc. ------------------------------------------------

  if((p == TIMER1_A_PIN) || (p == TIMER1_B_PIN)) { // Valid pin?
    pin   = p;                                     // Yes, save
    ocr   = (servoPos_t *)&((p == TIMER1_A_PIN) ? OCR1A : OCR1B);
#if (F_CPU == 16000000L)
    GTCCR = _BV(PWM1B);                            // Enable PWM A+B
    TCCR1 = _BV(PWM1A) | _BV(CS13) | _BV(CS12);    // 1:2048 prescale
    OCR1C = F_CPU / 2048 / 50;                     // 156 = ~50 Hz
    OCR1A = OCR1B = F_CPU / 2048 / 50 * 3 / 40;    // Center servos
    if(TCNT1 > OCR1C) TCNT1 = 0;                   // Restart count
#else
    GTCCR = _BV(PWM1B);
    TCCR1 = _BV(PWM1A) | _BV(CS13) | _BV(CS11) | _BV(CS10); // 1:1024
    OCR1C = F_CPU / 1024 / 50;
    OCR1A = OCR1B = F_CPU / 1024 / 50 * 3 / 40;
    if(TCNT1 > OCR1C) TCNT1 = 0;
#endif
  }

#elif defined (__AVR_ATmega168__) || defined (__AVR_ATmega328P__)
  // Arduino Uno, Duemilanove, etc. ----------------------------------------

  if((p == TIMER1_A_PIN) || (p == TIMER1_B_PIN)) { // Valid pin?
    pin    = p;                                    // Yes, save
    ocr    = (servoPos_t *)&((p == TIMER1_A_PIN) ? OCR1A : OCR1B);
    TCCR1A = _BV(WGM11);                           // Mode 14 (fast PWM)
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);  // 1:8 prescale
    ICR1   = F_CPU / 8 / 50;                       // ~50 Hz
    OCR1A  = OCR1B = F_CPU / 8 / 50 * 3 / 40;      // Center servos
    if(TCNT1 > ICR1) TCNT1 = 0;                    // Restart count
  }

#elif defined(__AVR_ATmega32U4__)
  // Arduino Leonardo, Teensy 2.0 ------------------------------------------

  if((p == TIMER1_A_PIN) || (p == TIMER1_B_PIN) || (p == TIMER1_C_PIN)) {
    pin     = p;
    counter = (servoPos_t *)&TCNT1;
    if(p == TIMER1_A_PIN)      ocr = (servoPos_t *)&OCR1A;
    else if(p == TIMER1_B_PIN) ocr = (servoPos_t *)&OCR1B;
    else                       ocr = (servoPos_t *)&OCR1C;
    TCCR1A  = _BV(WGM11);                              // Mode 14 (fast PWM)
    TCCR1B  = _BV(WGM13) | _BV(WGM12) | _BV(CS11);     // 1:8 prescale
    ICR1    = F_CPU / 8 / 50;                          // ~50 Hz
    OCR1A   = OCR1B = OCR1C = F_CPU / 8 / 50 * 3 / 40; // Center servos
    if(TCNT1 > ICR1) TCNT1 = 0;                        // Restart count
  } else if(p == TIMER3_A_PIN) {
    pin     = p;
    ocr     = (servoPos_t *)&OCR3A;
    counter = (servoPos_t *)&TCNT3;
    TCCR3A  = _BV(WGM31);
    TCCR3B  = _BV(WGM33) | _BV(WGM32) | _BV(CS31);
    ICR3    = F_CPU / 8 / 50;
    OCR3A   = F_CPU / 8 / 50 * 3 / 40;
    if(TCNT3 > ICR3) TCNT3 = 0;
  }

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  // Arduino Mega ----------------------------------------------------------

  switch(p) {
   case TIMER1_A_PIN:
   case TIMER1_B_PIN:
   case TIMER1_C_PIN:
    pin     = p;
    counter = (servoPos_t *)&TCNT1;
    if     (p == TIMER1_A_PIN) ocr = (servoPos_t *)&OCR1A;
    else if(p == TIMER1_B_PIN) ocr = (servoPos_t *)&OCR1B;
    else                       ocr = (servoPos_t *)&OCR1C;
    TCCR1A  = _BV(WGM11);                              // Mode 14 (fast PWM)
    TCCR1B  = _BV(WGM13) | _BV(WGM12) | _BV(CS11);     // 1:8 prescale
    ICR1    = F_CPU / 8 / 50;                          // ~50 Hz
    OCR1A   = OCR1B = OCR1C = F_CPU / 8 / 50 * 3 / 40; // Center servos
    if(TCNT1 > ICR1) TCNT1 = 0;                        // Restart count
    break;
   case TIMER3_A_PIN:
   case TIMER3_B_PIN:
   case TIMER3_C_PIN:
    pin     = p;
    counter = (servoPos_t *)&TCNT3;
    if     (p == TIMER3_A_PIN) ocr = (servoPos_t *)&OCR3A;
    else if(p == TIMER3_B_PIN) ocr = (servoPos_t *)&OCR3B;
    else                       ocr = (servoPos_t *)&OCR3C;
    TCCR3A  = _BV(WGM31);
    TCCR3B  = _BV(WGM33) | _BV(WGM32) | _BV(CS31);
    ICR3    = F_CPU / 8 / 50;
    OCR3A   = OCR3B = OCR3C = F_CPU / 8 / 50 * 3 / 40;
    if(TCNT3 > ICR3) TCNT3 = 0;
    break;
   case TIMER4_A_PIN:
   case TIMER4_B_PIN:
   case TIMER4_C_PIN:
    pin     = p;
    counter = (servoPos_t *)&TCNT4;
    if     (p == TIMER4_A_PIN) ocr = (servoPos_t *)&OCR4A;
    else if(p == TIMER4_B_PIN) ocr = (servoPos_t *)&OCR4B;
    else                       ocr = (servoPos_t *)&OCR4C;
    TCCR4A  = _BV(WGM41);
    TCCR4B  = _BV(WGM43) | _BV(WGM42) | _BV(CS41);
    ICR4    = F_CPU / 8 / 50;
    OCR4A   = OCR4B = OCR4C = F_CPU / 8 / 50 * 3 / 40;
    if(TCNT4 > ICR4) TCNT4 = 0;
    break;
   case TIMER5_A_PIN:
   case TIMER5_B_PIN:
   case TIMER5_C_PIN:
    pin     = p;
    counter = (servoPos_t *)&TCNT5;
    if     (p == TIMER5_A_PIN) ocr = (servoPos_t *)&OCR5A;
    else if(p == TIMER5_B_PIN) ocr = (servoPos_t *)&OCR5B;
    else                       ocr = (servoPos_t *)&OCR5C;
    TCCR5A  = _BV(WGM51);
    TCCR5B  = _BV(WGM53) | _BV(WGM52) | _BV(CS51);
    ICR5    = F_CPU / 8 / 50;
    OCR5A   = OCR5B = OCR5C = F_CPU / 8 / 50 * 3 / 40;
    if(TCNT5 > ICR5) TCNT5 = 0;
    break;
  }

#endif
}

// -------------------------------------------------------------------------
// Enable or disable PWM on pin.  Constructor already set up control
// registers for no output, so switching on or off just needs to toggle
// bit difference between off & non-inverting mode.
// -------------------------------------------------------------------------
void Adafruit_NeoServo::toggle(void) {

#if defined(__AVR_ATtiny85__)
  // Adafruit Trinket, etc. ------------------------------------------------

  if(pin == TIMER1_A_PIN) TCCR1 ^= _BV(COM1A1);
  else                    GTCCR ^= _BV(COM1B1);

#elif defined (__AVR_ATmega168__) || defined (__AVR_ATmega328P__)
  // Arduino Uno, Duemilanove, etc. ----------------------------------------

  if(pin == TIMER1_A_PIN) TCCR1A ^= _BV(COM1A1);
  else                    TCCR1A ^= _BV(COM1B1);

#elif defined(__AVR_ATmega32U4__)
  // Arduino Leonardo, Teensy 2.0 ------------------------------------------

  switch(pin) {
   case TIMER1_A_PIN: TCCR1A ^= _BV(COM1A1); break;
   case TIMER1_B_PIN: TCCR1A ^= _BV(COM1B1); break;
   case TIMER1_C_PIN: TCCR1A ^= _BV(COM1C1); break;
   case TIMER3_A_PIN: TCCR3A ^= _BV(COM3A1); break;
  }

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  // Arduino Mega ----------------------------------------------------------

  switch(pin) {
   case TIMER1_A_PIN: TCCR1A ^= _BV(COM1A1); break;
   case TIMER1_B_PIN: TCCR1A ^= _BV(COM1B1); break;
   case TIMER1_C_PIN: TCCR1A ^= _BV(COM1C1); break;
   case TIMER3_A_PIN: TCCR3A ^= _BV(COM3A1); break;
   case TIMER3_B_PIN: TCCR3A ^= _BV(COM3B1); break;
   case TIMER3_C_PIN: TCCR3A ^= _BV(COM3C1); break;
   case TIMER4_A_PIN: TCCR4A ^= _BV(COM4A1); break;
   case TIMER4_B_PIN: TCCR4A ^= _BV(COM4B1); break;
   case TIMER4_C_PIN: TCCR4A ^= _BV(COM4C1); break;
   case TIMER5_A_PIN: TCCR5A ^= _BV(COM5A1); break;
   case TIMER5_B_PIN: TCCR5A ^= _BV(COM5B1); break;
   case TIMER5_C_PIN: TCCR5A ^= _BV(COM5C1); break;
  }

#endif
}

// -------------------------------------------------------------------------
// Set servo position.  Positions are always 'raw' in timer/counter units,
// there is no built-in mapping behavior.  This is on purpose and by design,
// because every sketch, servo and situation requires unique constraints;
// library sets mechanism, not policy.  Sketch should use Arduino map()
// function or its own techniques for scaling servo pos to timer counts.
// -------------------------------------------------------------------------
void Adafruit_NeoServo::write(const servoPos_t pos) {

  if(pin < 0) return;   // Invalid pin passed to attach()

  *ocr = pos;           // Set output compare register
  if(active) return;    // If pin already enabled, we're done

  // ...otherwise, this is the first time accessing this pin.
  // Wait for timer to be past servo pulse before enabling output,
  // avoids half-pulse that could throw off initial servo position.
#ifdef TIMER3_A_PIN     // For chips with >1 timer/counter
  while(*counter < pos);
#else
  while(TCNT1 < pos);
#endif

  toggle();             // Enable PWM on pin
  pinMode(pin, OUTPUT); // Enable output
  active = true;        // Mark pin as enabled
}

// -------------------------------------------------------------------------
// Disable PWM output on pin
// -------------------------------------------------------------------------
void Adafruit_NeoServo::detach(void) {

  if((pin < 0) || !active) return;  // Pin not previously used; ignore

  // Wait for timer to be past servo pulse before disabling output,
  // avoids half-pulse that could throw off final servo position.
#ifdef TIMER3_A_PIN    // For chips with >1 timer/counter
  while(*counter < *ocr);
#else
  while(TCNT1 < *ocr);
#endif

  pinMode(pin, INPUT); // Disable output
  toggle();            // Disable PWM on pin
  active = false;      // Mark as stopped
}
