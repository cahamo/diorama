/*
 * 1st of two tests of lighting LEDs using a CD74HC4067 16 channel multiplexer
 * 
 * This test was performed to help determine the best way to light the diorama's LEDs
 * while minimising the number of micro-controller pins that are used.
 * 
 * Connections
 * -----------
 * 
 * | Arduino | CD74HC4067 | Notes
 * +---------+------------+------
 * | pin     | pin        |
 * | GND     | GND        | The circuit worked without this connection
 * | 5V      | VCC        | The circuit worked without this connection
 * | D3      | EN         | This connection is only required to switch all LEDs off
 * | D8      | S0         |
 * | D9      | S1         |
 * | D10     | S2         |
 * | D11     | S3         |
 * | D12     | SIG        |
 * +---------+------------+
 * 
 * Each of the CD74HC4067's pins C0..C15 was connected via a 470ohm resistor to the anode of a LED.
 * The LEDs' cathodes were commoned and then connected to an Arduino GND pin.
 * 
 * This code repeatedly:
 *   - lights each of 16 LEDs in sequence, with 0.1 sec delay after each using the S0..S3 pins
 *   - switches everything off for 0.5sec using the EN pin
 */

const int selectPins[] = {8, 9, 10, 11};  // Arduino pins connected to CD74HC4067's pins S0..S3 
const int sigPin = 12;                    // Arduino pin connected to CD74HC4067's SIG pin
const int enPin = 3;                      // Arduino pin connected to CD74HC4067's EN pin

byte led = 0; // LED currently being lit: range 0..15 corresponds to CD74HC4067 pins C0..C15

void setup() {
  // Set up select pins S0..S3, the SIG pin & EN pin as outputs and set them low
  for (int i = 0; i < 4; i++) {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], LOW);
  }
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, LOW);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
}

void loop() {
  // Setup select pins S0..S3 to address output pin connected to current LED
  for (int i = 0; i < 4; i++) {
    digitalWrite(
      selectPins[i], 
      (led & (1 << i)) ? HIGH : LOW 
    );
  }
  // Light the slected LED, wait a bit, then turn it off
  digitalWrite(sigPin, HIGH);
  delay(100);
  digitalWrite(sigPin, LOW);

  // Move to next LED
  led = (led + 1) % 16;

  // If we've gone through all the LEDs, turn everything off for 1/2 sec
  if (led == 0) {
    digitalWrite(enPin, HIGH);
    delay(500);
    digitalWrite(enPin, LOW);
  }
  
}
