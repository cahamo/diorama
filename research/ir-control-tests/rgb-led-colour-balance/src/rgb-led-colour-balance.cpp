/*
 * Test code for use when calculating resistor values for the various pins of a
 * RGB LED to balance the colour of the red, green & blue led chips.
 *
 * Displays various colours in the RGB LED in a repeating sequence.
 *
 * While the code is running various resistances can be tried to achieve well
 * balanced colours.
 */

#include <Arduino.h>

#define FEEDBACK_RED_PIN 9
#define FEEDBACK_GREEN_PIN 10
#define FEEDBACK_BLUE_PIN 6

#define DELAY_TIME 500

// Set LED colour to given r(ed), g(green) & b(lue) values, using PWM.
void setLEDColour(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(FEEDBACK_RED_PIN, r);
  analogWrite(FEEDBACK_GREEN_PIN, g);
  analogWrite(FEEDBACK_BLUE_PIN, b);
}

void setup() {
  pinMode(FEEDBACK_RED_PIN, OUTPUT);
  pinMode(FEEDBACK_GREEN_PIN, OUTPUT);
  pinMode(FEEDBACK_BLUE_PIN, OUTPUT);
}

void loop() {
  setLEDColour(255, 0, 0);
  delay(DELAY_TIME);
  setLEDColour(0, 255, 0);
  delay(DELAY_TIME);
  setLEDColour(192, 192, 0);
  delay(DELAY_TIME);
  setLEDColour(0, 0, 255);
  delay(DELAY_TIME);
  setLEDColour(192, 192, 192);
  delay(DELAY_TIME);
}
