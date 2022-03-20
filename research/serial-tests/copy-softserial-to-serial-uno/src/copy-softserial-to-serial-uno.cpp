// Copies input from SoftwareSerial to Serial output.
// Both serial ports are run at the same speed.

// Simple code used to test serial communications from another microcontroller
// by copying the received data to Serial for display in a terminal.

// RX_PIN and TX_PIN define the pins to connect for RX and
// TX on the Uno. GND of the Uno should be connected to
// the GND of the other microcontroller.

// Can be tested with an Arduino Nano that is running the
// copy-softserial-to-serial-uno project.

// Created 2022 by Cahamo (https://cahamo.github.io)
// This code is placed in the public domain
// The SoftwareSerial library is LGPL v2.1 or later

#include <Arduino.h>
#include <SoftwareSerial.h>

#define RX_PIN 10
#define TX_PIN 11

#define BAUD_RATE 9600

SoftwareSerial mySerial = SoftwareSerial(RX_PIN, TX_PIN);


void setup() {
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  mySerial.begin(BAUD_RATE);
  Serial.begin(BAUD_RATE);
  Serial.println("READY");
  Serial.println("=====");
}

void loop() {
  char ch;
  // put your main code here, to run repeatedly:
  if (mySerial.available()) {
    ch = mySerial.read();
    Serial.print(ch);
  }
}
