/*
 * Test of lighting 16 LEDs using two 74HC595 shift registers and an Arduino Uno
 *
 * This test was performed to help determine the best way to light the diorama's LEDs
 * while minimising the number of micro-controller pins that are used.
 *
 * Connections
 * ===========
 *
 * Place 16 LEDs on a breadboard, each with a 1000ohm current limiting resitor, all with the
 * cathodes commoned to an Arduino GND pin and the anodes connected to a shift register
 * output (Qn, 0<=n<=7).
 *
 * Make connections to the shift registers as follows:
 *
 * Shift register 1
 * ----------------
 *
 * Pin # | Pin Name | Connect to
 * ------+----------+---------
 *   1   |   Q1     | LED #2 anode
 *   2   |   Q2     | LED #3 anode
 *   3   |   Q3     | LED #4 anode
 *   4   |   Q4     | LED #5 anode
 *   5   |   Q5     | LED #6 anode
 *   6   |   Q6     | LED #7 anode
 *   7   |   Q7     | LED #8 anode
 *   8   |   GND    | Arduino Uno GND pin
 *   9   |   Q7'    | Shift register 2 pin 14 (DS)
 *  10   |   MR     | Arduino Uno 5V pin
 *  11   |  SH_CP   | Arduino Uno pin D12
 *  12   |  ST_CP   | Arduino Uno pin D8
 *  13   |   OE     | Arduino Uno GND pin
 *  14   |   DS     | Arduino Uno pin D11
 *  15   |   Q0     | LED #1 anode
 *  16   |   VCC    | Arduino Uno 5V pin
 *
 * Shift register 2
 * ----------------
 *
 * Pin # | Pin Name | Connect to
 * ------+----------+---------
 *   1   |   Q1     | LED #10 anode
 *   2   |   Q2     | LED #11 anode
 *   3   |   Q3     | LED #12 anode
 *   4   |   Q4     | LED #13 anode
 *   5   |   Q5     | LED #14 anode
 *   6   |   Q6     | LED #15 anode
 *   7   |   Q7     | LED #16 anode
 *   8   |   GND    | Arduino Uno GND pin
 *   9   |   Q7'    | Not connected
 *  10   |   MR     | Arduino Uno 5V pin
 *  11   |  SH_CP   | Arduino Uno pin D12
 *  12   |  ST_CP   | Arduino Uno pin D8
 *  13   |   OE     | Arduino Uno GND pin
 *  14   |   DS     | Shift register 1 pin 9 (Q7')
 *  15   |   Q0     | LED #9 anode
 *  16   |   VCC    | Arduino Uno 5V pin
 *
 */

#include <Arduino.h>

#define DATA_PIN 11   // => 74HC595 pin 14 (DS)
#define CLOCK_PIN 12  // => 74HC595 pin 11 (SH_CP)
#define LATCH_PIN 8   // => 74HC595 pin 12 (ST_CP)

#define NUM_LEDS 16
#define ON true
#define OFF false

// Define a sequence of patterns of LEDs as binary numbers: 0 => off, 1=> on
const uint16_t sequence[] = {
  0b0000000000000000,
  0b1000000000000001,
  0b0100000000000010,
  0b0010000000000100,
  0b0001000000001000,
  0b0000100000010000,
  0b0000010000100000,
  0b0000001001000000,
  0b0000000110000000,
  0b0000001001000000,
  0b0000010000100000,
  0b0000100000010000,
  0b0001000000001000,
  0b0010000000000100,
  0b0100000000000010,
  0b1000000000000001,
  0b0000000000000000,
  0b1111111111111111,
  0b0000000000000000,
  0b1111111111111111,
  0b0000000000000000,
  0b1111111100000000,
  0b0000000000000000,
  0b0000000011111111,
  0b0000000000000000,
  0b0011001100110011,
  0b1100110011001100,
  0b0011001100110011,
  0b1100110011001100,
  0b0000000000000000,
  0b0000000000000001,
  0b0000000000000011,
  0b0000000000000110,
  0b0000000000001100,
  0b0000000000011000,
  0b0000000000110000,
  0b0000000001100000,
  0b0000000011000000,
  0b0000000110000000,
  0b0000001100000000,
  0b0000011000000000,
  0b0000110000000000,
  0b0001100000000000,
  0b0011000000000000,
  0b0110000000000000,
  0b1100000000000000,
  0b1000000000000000,
  0b0000000000000000,
  0b0101010101010101,
  0b1010101010101010,
  0b0101010101010101,
  0b1010101010101010,
  0b0000000000000000,
  0b0000000000001111,
  0b0000000011110000,
  0b0000111100000000,
  0b1111000000000000,
  0b0000111100000000,
  0b0000000011110000,
  0b0000000000001111,
  0b0000000000000000
};

#define SEQUENCE_LENGTH (sizeof(sequence) / sizeof(sequence[0]))

void setData(uint16_t data) {
  // set up shift registers to light the required LEDs
  byte lsByte = data & 0x00FF;          // byte of data for 1st shift register
  byte msByte = (data & 0xFF00) >> 8;   // byte of data for 2nd shift register
  // send the data to the shift registers
  // set latch low to prevent flicker as data is shifted in
  digitalWrite(LATCH_PIN, LOW);
  // shift out msByte: this gets intially gets stored in 1st shift register
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, msByte);
  // shift out lsByte: this causes msByte to be shifted over into 2nd register
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, lsByte);
  // latch on: lights the LEDs according to pattern defined by data
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  // Set all control pins to output
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
}

void loop() {
  // uint16_t data;
  // Loop through all sequences, with delay of 200ms between them, then start over on next loop() call
  for (unsigned int i = 0; i < SEQUENCE_LENGTH; i++) {
    setData(sequence[i]);
    delay(200);
  }
}
