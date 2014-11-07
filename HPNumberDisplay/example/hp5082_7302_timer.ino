/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/*
  hp5082-7302.ino - An example application that shows how the
  TimeCounter and HPNumberDisplay libraries are used.

  It requires jspark311's Scheduler library
  (https://github.com/jspark311/Scheduler) and the TimerOne library
  (https://code.google.com/p/arduino-timerone/).
 */

#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Scheduler.h>
#include <TimerOne.h>

#include <HPNumberDisplay.h>
#include <TimeCounter.h>

//Pin definitions
namespace IO
{
  enum OutPins
    {
      TIME_RESET_LED = 13,
      TIME_LED       = 8,
      
      INPUT1         = 14,
      INPUT2         = 15,
      INPUT4         = 16,
      INPUT8         = 17,
      DP             = 18,

      ENABLE1        = 7,
      ENABLE2        = 9,
      ENABLE3        = 10,
      ENABLE4        = 19,
      ENABLE5        = 5,
    };

  enum InPins
    {
      TIME_RESET = 6
    };
  
  const uint8_t enable_pins[5] = {ENABLE1, ENABLE2, ENABLE3, ENABLE4, ENABLE5};
}

Scheduler scheduler;
TimeCounter counter;
HPNumberDisplay display(5, IO::enable_pins, IO::INPUT1, IO::INPUT2, IO::INPUT4, IO::INPUT8, IO::DP);

/*
  Returns the nth digit in a number
 */
uint8_t getDigit(uint8_t number, int digit)
{
  static uint8_t powersof10[] = {1, 10, 100};
  return ((number / powersof10[digit]) % 10);
}


/*
  Sends the current timer values to the display.
 */
void updateDisplay()
{
  uint8_t hours = counter.getHours();
  uint8_t minutes = counter.getMinutes();
  uint8_t seconds = counter.getSeconds();

  display.set(getDigit(hours,   0), false, 0);
  display.set(getDigit(minutes, 1), true,  1);
  display.set(getDigit(minutes, 0), false, 2);
  display.set(getDigit(seconds, 1), true,  3);
  display.set(getDigit(seconds, 0), false, 4);
}

/*
  Fills the display with '-'.
 */
void clearDisplay()
{
  display.set(HPNumberDisplay::MINUS, false, 0);
  display.set(HPNumberDisplay::MINUS, false, 1);
  display.set(HPNumberDisplay::MINUS, false, 2);
  display.set(HPNumberDisplay::MINUS, false, 3);
  display.set(HPNumberDisplay::MINUS, false, 4);
}

/*
  Check if the reset buton is pushed.
 */
void poll_button_task()
{
  if(digitalRead(IO::TIME_RESET) == LOW)
    {
      //Reset counter and clear the display.
      counter.setTime(0, 0, 0);
      clearDisplay();

      digitalWrite(IO::TIME_RESET_LED, HIGH);
    }
  else
    {
      digitalWrite(IO::TIME_RESET_LED, LOW);
    }
}

/*
  Increment the time counter by one second.
 */
void second_count_task()
{
  digitalWrite(IO::TIME_LED, !(digitalRead(IO::TIME_LED) == HIGH));
  counter.increment();
  updateDisplay();
}

/*
  Update scheduler
 */
void timerCallbackScheduler() {
  scheduler.advanceScheduler();
}

void setup() {
  //Initilise pins
  pinMode(IO::TIME_RESET_LED, OUTPUT);
  pinMode(IO::TIME_LED, OUTPUT);
  pinMode(IO::TIME_RESET, INPUT);

  scheduler.createSchedule(1000, -1, true, second_count_task);
  scheduler.createSchedule(100, -1, true, poll_button_task);

  //Initilise timer interrupts
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerCallbackScheduler);
  sei();
}

void loop() {
  scheduler.serviceScheduledEvents();
}

