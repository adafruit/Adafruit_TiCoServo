/*------------------------------------------------------------------------
  Adafruit_NeoServo example for ATmega-based boards such as the Arduino
  Uno, Leonardo, Mega, Duemilanove and compatible boards such as the
  Adafruit Pro Trinket, Flora or PJRC Teensy 2.0 (not Trinket or Gemma,
  see other example for that).

  THE GOOD NEWS:
  - NeoPixels and servos in the same sketch!

  THE BAD NEWS:
  - Limited to 2 servos on the most popular boards (4-12 on others).
  - Servos must be on specific pins. This is explained later in the code.
  - Supports the most common AVR-based boards, but not everything.
  - Uses up Timer/Counter 1, a popular choice of other time-critical
    libraries such as WaveHC. Can only do so much in one sketch.
    Some boards (Leonardo, Mega) have additional 16-bit timers available.

  - Servo position is specified in raw timer/counter units, which isn't
    necessarily good or bad news, just a design choice. Every servo is
    different and projects usually have a calibration phase anyway, so
    the library doesn't do anything to limit or exaggerrate resolution.
    You can use the Arduino map() function to translate from one unit
    type to another...this is demonstrated later in this example.

  Example reads a potentiometer on pin A0, moves a servo (pin 9) and
  fills a portion of a NeoPixel strip (pin 6) to match.
  ------------------------------------------------------------------------*/

#if defined(__AVR_ATtiny85__)
 #error "This code is for ATmega boards, see other example for ATtiny."
#endif
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoServo.h>

// NeoPixel parameters. These are configurable, but the pin number must
// be different than the servo(s).
#define N_LEDS       60
#define LED_PIN       6

// Servo parameters. These have limited configurability. Compatible pins,
// for example, are determined by the specific hardware. On Arduino Uno,
// Diecimila and Adafruit Pro Trinket, pins 9 and 10 are supported by the
// library. On Arduino Leonardo: pins 5, 9, 10 and 11. Adafruit Flora:
// pins D9, D10. PJRC Teensy 2.0 (not Teensy++ or 3.X): pins 4, 9, 14, 15.
// Arduino Mega: 2, 3, 5, 6, 7, 8, 11, 12, 13, 44, 45, 46.
// As mentioned earlier, servo position is stated in raw timer/counter
// units, which will vary with clock speed. The #defines below provide
// estimated min/max pulse durations for many servos, but for maximum
// control you'll probably need to do some math and/or testing to find the
// actual range for your specific servos.
// On a 16 MHz MCU, 1 unit = 0.5 us, 1 ms = 2000 units, 2 ms = 4000.
// 12 MHz: 1 unit = 0.67 us, 1 ms = 1500 units, 2 ms = 3000.
// 8 MHz: 1 unit = 1.0 us, 1 ms = 1000 units, 2 ms = 2000.
// Use the Arduino map() function to scale between your code's positional
// units (e.g. 0-1023 from analogRead()) to the actual calibrated min/max
// pulse times determined for your servos & MCU clock speed.
#define SERVO_PIN     9
#define SERVO_MIN    (F_CPU / 8 / 50 / 20)         // ~1 ms pulse
#define SERVO_MAX    (SERVO_MIN * 2)               // ~2 ms pulse
#define SERVO_CENTER ((SERVO_MIN + SERVO_MAX) / 2) // ~1.5 ms

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN);
Adafruit_NeoServo servo;

void setup(void) {
  servo.begin(SERVO_PIN);
  strip.begin();
}

void loop(void) {
  int a, x;
  a = analogRead(0);                            // 0 to 1023
  x = map(a, 0, 1023, SERVO_MIN, SERVO_MAX);    // Scale to servo range
  servo.move(x);                                // Move servo
  x = map(a, 0, 1023, 0, strip.numPixels()-1);  // Scale to strip length
  strip.clear();
  while(x--) strip.setPixelColor(x, 255, 0, 0); // Set pixels
  strip.show();                                 // Update strip
}
