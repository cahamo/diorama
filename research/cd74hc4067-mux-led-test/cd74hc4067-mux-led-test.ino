/*
 * Test of lighting LEDs using a CD74HC4067 16 channel multiplexer
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
 * Each of the CD74HC4067's pins C0..C15 was connected via a 460ohm resistor to the anode of a LED.
 * The LEDs' cathodes were commoned and then connected to an Arduino GND pin.
 * 
 * This code repeatedly:
 *   - lights each of 16 LEDs in sequence, with 0.1 sec delay after each using the S0..S3 pins
 *   - switches everything off for 0.5sec using the EN pin
 */

const int selectPins[] = {8, 9, 10, 11};
const int sigPin = 12;
const int enPin = 3;
byte controlPins[] = {
                  B00000000, 
                  B00000001,
                  B00000010,
                  B00000011,
                  B00000100,
                  B00000101,
                  B00000110,
                  B00000111,
                  B00001000,
                  B00001001,
                  B00001010,
                  B00001011,
                  B00001100,
                  B00001101,
                  B00001110,
                  B00001111 };

int curControlPin = 0;

void setup() {
  // Set up the select pins, as outputs
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
  for (int i = 0; i < 4; i++) {
    digitalWrite(
      selectPins[i], 
      (controlPins[curControlPin] & (1 << i)) ? HIGH : LOW 
    );
  }
  digitalWrite(sigPin, HIGH);

  curControlPin++;
  curControlPin %= 16;

  delay(100);
  digitalWrite(sigPin, LOW);

  if (curControlPin == 0) {
    digitalWrite(enPin, HIGH);
    delay(500);
    digitalWrite(enPin, LOW);
  }
  
}
