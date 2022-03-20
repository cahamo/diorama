// TinyWireM Master Writer
// by cahamo
// Based upon
//    Wire Master Writer
//    by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the TinyWireM library
// Writes data to an I2C/TWI Peripheral device
// Refer to Nicholas Zambetti's "Wire Peripheral Receiver" example for use with this

// TinyWireM expects the following pins to be used on the ATtiny85:
// PB0 (pin 5) = SDA
// PB2 (pin 7) = SCK
// This is hard wired in USI_TWI_Master.h:
//    #define PIN_USI_SDA PINB0
//    #define PIN_USI_SCL PINB2

// Created 2022 by Cahamo (https://cahamo.github.io)
// This example code is in the public domain.
// The TinyWireM library code is GPL v2.1 or later.


#include <Arduino.h>
#include <TinyWireM.h>

void setup()
{
  TinyWireM.begin(); // join i2c bus as master
  // Wire.begin();  (address optional for master)
}

byte x = 0;

void loop()
{
  TinyWireM.beginTransmission(4);  // transmit to device #4
  // Replaced Wire.write("x is ") line with 5 statements below
  // ... TinyWireM has no char[] overload for write()
  // Wire.write("x is ");        // sends five bytes
  TinyWireM.write('x');
  TinyWireM.write(' ');
  TinyWireM.write('i');
  TinyWireM.write('s');
  TinyWireM.write(' ');
  // Back to direct copy of Wire library version
  TinyWireM.write(x);              // sends one byte
  TinyWireM.endTransmission();     // stop transmitting

  x++;
  delay(500);
}
