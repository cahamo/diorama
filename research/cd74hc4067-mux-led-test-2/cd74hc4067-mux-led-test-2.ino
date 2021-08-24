/*
 * 2nd of two tests of lighting LEDs using a CD74HC4067 16 channel multiplexer
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
 * | GND     | GND        | The circuit worked without this connection but LEDs were dimmer
 * | 5V      | VCC        | The circuit worked without this connection without any differences
 * | D8      | S0         |
 * | D9      | S1         |
 * | D10     | S2         |
 * | D11     | S3         |
 * | D12     | SIG        |
 * +---------+------------+
 * 
 * Each of the CD74HC4067's pins C0..C15 was connected the anode of a LED. The cathode of each LED was
 * connected to a 470ohm resistor. The resistors were all connected to ground, back to the Arduino GND pin.
 * 
 * This code displays various patterns of LEDs.
 * 
 * NOTE:
 *    Unlike in part one of the test, the CD74HC4067's EN pin is not manipulated by this sketch. It was 
 *    therefore disconnected from the circuit and left floating. It may have been better practise to couple
 *    pin the to ground.
 */

const int selectPins[] = {8, 9, 10, 11};  // Arduino pins connected to CD74HC4067's pins S0..S3
const int sigPin = 12;                    // Arduino pin connected to CD74HC4067's SIG pin

uint16_t patterns[] = { // Various LED patterns to be displayed
  0b1111000011110000,
  0b0000111100001111,
  0b1010101010101010,
  0b0101010101010101,
  0b1100110011001100,
  0b0011001100110011,
  0b1111111111111111,
  0b1000000000000001,
  0b0000100000010000,
  0b0000000110000000 
};

const unsigned int patternCount = sizeof(patterns) / sizeof(patterns[0]);
unsigned int patternIdx = 0;

void setup() {
  // Set up select pins S0..S3 & SIG pin as outputs and set them low
  for (int i = 0; i < 4; i++) {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], LOW);
  }
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, LOW);
}

void loop() {
  // loop() displays a sequence of patterns of illuminated LEDs
  
  uint16_t display;
  static unsigned long lastChange = 0;
  const unsigned long delta = 800;

  // Every "delta" milliseconds or so we advance to the next pattern of LEDs
  if (millis() - lastChange > delta) {
    lastChange = millis();
    patternIdx = (patternIdx + 1) % patternCount;
  }

  // Display the current pattern of LEDs
  show(patterns[patternIdx]);
}

void show(uint16_t v) {
  // show() displays the given value v as a LED pattern where a LED is illuminated if the
  // corresponding bit of v is set
  
  const int d = 2;  // delay time in ms

  // Loop through all LEDs
  for (byte led = 0; led < 16; led++) {
    // Setup CD74HC4067 pins S0..S3 to address output pin connected to current LED
    for (byte selPin = 0; selPin < 4; selPin++) {
      digitalWrite(selectPins[selPin], led & (1 << selPin));
    }
    // Light up the LED iff it corresponds to a set bit in v
    digitalWrite(sigPin, (v & (1 << led)) > 0 );
    // Delay a little: 
    //    this is needed to permit LEDs to gain some brightness before being switched off again
    delay(d);
  }
}
