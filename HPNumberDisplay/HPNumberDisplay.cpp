/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
  HPNumberDisplay.h - Library for controlling HP 5082-7300/7302
  numeric indicators. This library should also be able to control
  hexadecimal indicators (HP 5080-7340) by using the decimal point as
  blanking control, but this has not been tested.
*/

#include "Arduino.h"
#include <HPNumberDisplay.h>


HPNumberDisplay::HPNumberDisplay(uint8_t num_displays,
				 uint8_t enable_pins[],
				 uint8_t input1_pin,
				 uint8_t input2_pin,
				 uint8_t input4_pin,
				 uint8_t input8_pin,
				 uint8_t dp_pin) :
  _num_displays(num_displays),
  _i1(input1_pin), _i2(input2_pin),
  _i4(input4_pin), _i8(input8_pin),
  _dp(dp_pin)
{
  _enable_pins = new uint8_t[num_displays];
          
  for(uint8_t i = 0; i < _num_displays; ++i)
    {
      _enable_pins[i] = enable_pins[i];
      digitalWrite(_enable_pins[i], HIGH);
    }

  for(uint8_t i = 0; i < _num_displays; ++i)
    {
      set(BLANK, false, i);
    }
 }

HPNumberDisplay::~HPNumberDisplay()
{
  delete _enable_pins;
}

void HPNumberDisplay::set(uint8_t digit, bool decimal_point, uint8_t display)
{
  if(display < _num_displays)
    {
      //Write data on the input and db pins
      digitalWrite(_i1, bitRead(digit, 0));
      digitalWrite(_i2, bitRead(digit, 1));
      digitalWrite(_i4, bitRead(digit, 2));
      digitalWrite(_i8, bitRead(digit, 3));
      digitalWrite(_dp, !decimal_point);

      //Set enable latch to low, then high, to send data to the
      //display.
      digitalWrite(_enable_pins[display], LOW);
      //delayMicroseconds(1); //Needed at high speeds?
      digitalWrite(_enable_pins[display], HIGH);

      return;
    }
 }
