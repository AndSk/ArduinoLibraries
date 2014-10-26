/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
  TimeCounter.h - Library for counting time.
*/

#include "Arduino.h"

#ifndef TIMECOUNTER_H
#define TIMECOUNTER_H

/*
   TimeCounter can be used to count hours, minutes, and seconds. Set
   a time using the setTime method, then count up with the increment
   methods, or count down with the decrement methods.

   The starting time of a TimeCounter is 00:00:00.
   The maximum time is 255:59:59.
   The minimum time is 00:00:00.

   TimeCounter will stop at the maximum and minimum time. It does not
   wrap around.
 */
class TimeCounter
{
public:
  TimeCounter();

  /*
     Set a new time.

     Allowed values are:
     0 <= hours <= 255
     0 <= minutes <= 59
     0 <= seconds <= 59
   */
  void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

  /*
    Increments time by one second.
    Returns true if the maximum value was reached.
   */
  bool increment();

  /*
    Decrements time by one second.
    Returns true if the minimum value was reached.
   */
  bool decrement();

  /*
    Increments time. Any values are allowed.  TimeCounter will convert
    large second and minute values to minutes and hours and checks
    for integer overflow.
    
    Returns true if the maximum value was reached.
   */
  bool increment(uint8_t hours, uint8_t minutes, uint8_t seconds);

  /*
    Decrements time. Any values are allowed. TimeCounter will convert
    large second and minute values to minutes and hours and checks for
    integer underflow.
    
    Returns true if the minimum value was reached.
   */
  bool decrement(uint8_t hours, uint8_t minutes, uint8_t seconds);

  uint8_t getSeconds();
  uint8_t getMinutes();
  uint8_t getHours();
    
private:
  uint8_t _seconds, _minutes, _hours;
};

#endif
