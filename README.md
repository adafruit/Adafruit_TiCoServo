# Adafruit TiCoServo [![Build Status](https://github.com/adafruit/Adafruit_TiCoServo/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_TiCoServo/actions)

This is the Adafruit TiCoServo Arduino Library for Arduino

An alternate servo library when NeoPixels and servos are both needed in the same sketch (the stock Arduino Servo library stutters with the addition of NeoPixels). Has some limitations of its own, see examples for pros and cons. See Adafruit_TiCoServo.cpp for attribution.

This library only handles the servo control, you'll still need to install and use the NeoPixel library for the LEDs. It's not dependent on that library though, and could be used separately for other (non-NeoPixel) situations that might benefit from hardware-PWM-driven servos.

Compatible pins are determined by the specific hardware. 
* Arduino Uno, Diecimila and Adafruit Pro Trinket, pins 9 and 10 are supported.
* Arduino Leonardo: pins 5, 9, 10 and 11. 
* Adafruit Flora: pins D9, D10. 
* PJRC Teensy 2.0 (not Teensy++ or 3.X): pins 4, 9, 14, 15.
* Arduino Mega: pins 2, 3, 5, 6, 7, 8, 11, 12, 13, 44, 45, 46.



Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

# Dependencies
[Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)

# Contributing

Contributions are welcome! Please read our [Code of Conduct](https://github.com/adafruit/Adafruit_TiCoServo/blob/master/CODE_OF_CONDUCT.md>)
before contributing to help this project stay welcoming.

## Documentation and doxygen
Documentation is produced by doxygen. Contributions should include documentation for any new code added.

Some examples of how to use doxygen can be found in these guide pages:

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen-tips


Written by Phil Burgess for Adafruit Industries, incorporating work
by Paul Stoffregen, Jesse Tane, Jérôme Despatis, Michael Polli and
Dan Clemens. The file 'known_16bit_timers.h' is taken verbatim from
Paul Stoffregen's fork of the TimerOne library:
https://github.com/PaulStoffregen/TimerOne
and this code is provided under the same "CC BY 3.0 US" license terms.

Open Source License

Adafruit_TiCoServo is free software. You can redistribute it and/or
modify it under the terms of Creative Commons Attribution 3.0 United
States License. To view a copy of this license, visit
http://creativecommons.org/licenses/by/3.0/us/


To install, use the Arduino Library Manager and search for "Adafruit TiCoServo" and install the library.
