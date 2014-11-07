/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
  TimeCounter.cpp - Library for counting time.
*/


#include <TimeCounter.h>

TimeCounter::TimeCounter() : _seconds(0), _minutes(0), _hours(0) {}

void TimeCounter::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  if(minutes < 60 && seconds < 60)
    {
      _hours = hours;
      _minutes = minutes;
      _seconds = seconds;
    }
}
  
bool TimeCounter::increment()
{
  return increment(0, 0, 1);
}

bool TimeCounter::decrement()
{
  return decrement(0, 0, 1);
}

  
bool TimeCounter::increment(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  //Add hours
  
  //Check for overflow
  if(hours > 255 - _hours)
    {
      _hours = 255;
      _minutes = 59;
      _seconds = 59;
      
      return true;
    }

  _hours += hours;
  
  hours = 0;

  //Add minutes
  hours = minutes / 60;
  minutes = minutes % 60;

  //Divide this into two steps to remove the risk of overflow
  minutes += _minutes;
    
  hours += minutes / 60;
  _minutes = minutes % 60;
  minutes = 0;
    
  //Add seconds
  minutes = seconds / 60;
  seconds = seconds % 60;

  //Divide this into two steps to remove the risk of overflow
  seconds += _seconds;
    
  minutes += seconds / 60;
  _seconds = seconds % 60;
  seconds = 0;

  if(hours != 0 || minutes != 0)
    return increment(hours, minutes, seconds);
  else
    return false;
}

bool TimeCounter::decrement(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  //Subtract seconds

  //Check for underflow
  if(hours > _hours)
    {
      _hours = 0;
      _minutes = 0;
      _seconds = 0;
      
      return true;
    }

  _hours -= hours;
  hours = 0;

  //Subtract minutes
  hours = minutes / 60;
  minutes = minutes % 60;

  if(minutes > _minutes)
    {
      ++hours;
      _minutes = _minutes + (60 - minutes);
    }
  else
    {
      _minutes -= minutes;
    }

  minutes = 0;

  //Subtract seconds
  minutes = seconds / 60;
  seconds = seconds % 60;

  if(seconds > _seconds)
    {
      ++minutes;
      _seconds = _seconds + (60 - seconds);
    }
  else
    {
      _seconds -= seconds;
    }

  seconds = 0;

  if(hours != 0 || minutes != 0)
    return decrement(hours, minutes, seconds);
  else
    return false;
}
  

uint8_t TimeCounter::getSeconds() const
{
  return _seconds;
}

uint8_t TimeCounter::getMinutes() const
{
  return _minutes;
}

uint8_t TimeCounter::getHours() const
{
  return _hours;
}

