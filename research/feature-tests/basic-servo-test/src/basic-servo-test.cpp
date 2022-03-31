/*
 * Test drive of a single servo motor
 *
 * Based on "Sweep"
 *  by BARRAGAN <http://barraganstudio.com> (Public Domain).
 *  modified 8 Nov 2013 by Scott Fitzgerald
 *    http://www.arduino.cc/en/Tutorial/Sweep
 *  modified 26 Aug 2021 by Cahamo
 *    https://github.com/cahamo/
 *  modified 31 Mar 2022 by Cahamo (convert from Arduino IDE to PlatformIO)
 *    https://github.com/cahamo/
 *
 * This verion licensed under CC0 (public domain)
 *    https://creativecommons.org/share-your-work/public-domain/cc0/
 */

#include <Arduino.h>

// Include servo library
#include <Servo.h>

// Create servo object
Servo servo;

#define SERVO_PIN 9         // pin attached to servo

#define DELAY_LONG 1000
#define DELAY_SHORT 15      // required to let servo complete its motion

#define SERVO_MIN 0
#define SERVO_MAX 180
#define SERVO_HALFWAY 90

void setup() {
  // Attach the Servo variable to a pin:
  servo.attach(SERVO_PIN);

}

void loop() {
  // Move servo to specific angles
  servo.write(SERVO_MIN);
  delay(DELAY_LONG);
  servo.write(SERVO_HALFWAY);
  delay(DELAY_LONG);
  servo.write(SERVO_MAX);
  delay(DELAY_LONG);
  servo.write(SERVO_HALFWAY);
  delay(DELAY_LONG);

  // Sweep from minimum to maximum positions in 1 degree steps
  for (int angle = SERVO_MIN; angle <= SERVO_MAX; angle++) {
    servo.write(angle);
    delay(DELAY_SHORT);
  }

  // Sweep back from maximum to minimum positions in 1 degree steps
  for (int angle = SERVO_MAX; angle >= SERVO_MIN; angle--) {
    servo.write(angle);
    delay(DELAY_SHORT);
  }

  // Wait a bit before going round again
  delay(DELAY_LONG);
}
