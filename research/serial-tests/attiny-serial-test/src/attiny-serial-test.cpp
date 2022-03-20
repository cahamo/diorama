/*
 * attiny-serial-test.cpp
 * By Cahamo, 2022
 *
 * Tests using ATtinySerialOut library on ATtiny85
 *
 * Adapted from
 *    ATtinySerialOutExample.cpp
 *
 *    Copyright (C) 2015-2021  Armin Joachimsmeyer
 *    Email: armin.joachimsmeyer@gmail.com
 *
 * The original file that was adapted is part of TinySerialOut
 * https://github.com/ArminJo/ATtinySerialOut.
 *
 * TinySerialOut is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#include <Arduino.h> // required for main()

// #define TX_PIN here to change from default PB2
#include "ATtinySerialOut.hpp"

#define LED_PIN PB1   // ATTiny physical pin 6

void setup(void) {
  pinMode(LED_PIN, OUTPUT);
  initTXPin();
  Serial.println(F("START"));
}

void loop(void) {

  digitalWrite(LED_PIN, HIGH);
  writeString(F("LED On"));
  delay(1500);
  digitalWrite(LED_PIN, LOW);
  writeString(F("LED Off"));
  delay(500);
}
