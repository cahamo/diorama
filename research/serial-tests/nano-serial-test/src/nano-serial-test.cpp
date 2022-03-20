// Test for communicating from an Arduino Nano to another
// microcontroller using the SoftwareSerial library.

// Flashes the Nano's built in LED and sends the state of
// the LED over serial.

// RX_PIN and TX_PIN define the pins to connect for RX and
// TX on the Nano. GND of the Nano should be connected to
// the GND of the other microcontroller.

// Can be tested with an Arduino Uno that is running the
// nano-serial-test project.

// Created 2022 by Cahamo (https://cahamo.github.io)
// This code is placed in the public domain
// The SoftwareSerial library is LGPL v2.1 or later

#include <Arduino.h>

#include <SoftwareSerial.h>

#define LED_PIN LED_BUILTIN

#define RX_PIN 2
#define TX_PIN 3

SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  mySerial.begin(9600);
  delay(2000);
  mySerial.println(F("SoftwareSerial Test"));
  mySerial.println();
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  mySerial.println(F("LED On"));
  delay(1500);
  digitalWrite(LED_PIN, LOW);
  mySerial.println(F("LED Off"));
  delay(500);
}
