/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
  HPNumberDisplay.h - Library for controlling HP 5082-7300/7302
  numeric indicators. This library should also be able to control
  hexadecimal indicators (HP 5080-7340) by using the decimal point as
  blanking control, but this has not been tested.
*/

#ifndef HPNUMBERDISPLAY_H
#define HPNUMBERDISPLAY_H

/**
 *hp 5082-7302
 *
 */
class HPNumberDisplay
{
public:
  /*
    Non-numeric characters that the 73xx can display.
    TEST, MINUS, and BLANK can only be used with a 7300/7302.
    A-F can only be used with a 7340.
   */
  enum character 
    {
      TEST = 10,
      MINUS = 13,
      BLANK = 14,
      A = 10,
      B = 11,
      C = 12,
      D = 13,
      E = 14,
      F = 15
    };
  
    
  HPNumberDisplay(uint8_t num_displays, const uint8_t enable_pins[],
		  uint8_t input1_pin,uint8_t input2_pin,
		  uint8_t input4_pin,uint8_t input8_pin,
		  uint8_t dp_pin);

  ~HPNumberDisplay();

  /*
    Set a value to display.

    The decimal point flag can be used as blanking control on a 7430.
   */
  void set(uint8_t digit, bool decimal_point, uint8_t display);

private:
  uint8_t *_enable_pins;
  uint8_t _num_displays;
  uint8_t _i1, _i2, _i4, _i8;
  uint8_t _dp;  
};

#endif
