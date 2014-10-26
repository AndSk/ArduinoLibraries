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

//Status indicators
#define BUTTON_LED 13
#define TIME_LED 8

//Reset button
#define BUTTON_PIN 6

//Display inputs
#define INPUT1_PIN 14
#define INPUT2_PIN 15
#define INPUT4_PIN 16
#define INPUT8_PIN 17
#define DP_PIN 18

//Latch enable pins
#define ENABLE1_PIN 7
#define ENABLE2_PIN 9
#define ENABLE3_PIN 10
#define ENABLE4_PIN 19
#define ENABLE5_PIN 5

Scheduler scheduler;
TimeCounter counter;
HPNumberDisplay *display;

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

  display->set(getDigit(hours,   0), false, 0);
  display->set(getDigit(minutes, 1), true,  1);
  display->set(getDigit(minutes, 0), false, 2);
  display->set(getDigit(seconds, 1), true,  3);
  display->set(getDigit(seconds, 0), false, 4);
}

/*
  Fills the display with '-'.
 */
void clearDisplay()
{
  display->set(HPNumberDisplay::MINUS, false, 0);
  display->set(HPNumberDisplay::MINUS, false, 1);
  display->set(HPNumberDisplay::MINUS, false, 2);
  display->set(HPNumberDisplay::MINUS, false, 3);
  display->set(HPNumberDisplay::MINUS, false, 4);
}

/*
  Check if the reset buton is pushed.
 */
void poll_button_task()
{
  if(digitalRead(BUTTON_PIN) == LOW)
    {
      //Reset counter and clear the display.
      counter.setTime(0, 0, 0);
      clearDisplay();

      digitalWrite(BUTTON_LED, HIGH);
    }
  else
    {
      digitalWrite(BUTTON_LED, LOW);
    }
}

/*
  Increment the time counter by one second.
 */
void second_count_task()
{
  digitalWrite(TIME_LED, !(digitalRead(TIME_LED) == HIGH));
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
  pinMode(BUTTON_LED, OUTPUT);
  pinMode(TIME_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  pinMode(INPUT1_PIN, OUTPUT);
  pinMode(INPUT2_PIN, OUTPUT);
  pinMode(INPUT4_PIN, OUTPUT);
  pinMode(INPUT8_PIN, OUTPUT);
  pinMode(DP_PIN, OUTPUT);

  pinMode(ENABLE1_PIN, OUTPUT);
  pinMode(ENABLE2_PIN, OUTPUT);
  pinMode(ENABLE3_PIN, OUTPUT);
  pinMode(ENABLE4_PIN, OUTPUT);
  pinMode(ENABLE5_PIN, OUTPUT);

  //Initilise display
  uint8_t enable_pins[5] =  {ENABLE1_PIN, ENABLE2_PIN, ENABLE3_PIN, ENABLE4_PIN, ENABLE5_PIN};
  
  display = new HPNumberDisplay(5, enable_pins,
			  INPUT1_PIN, INPUT2_PIN, INPUT4_PIN, INPUT8_PIN, DP_PIN);
  
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

