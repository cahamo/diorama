/*
 * mock-i2c-command-receiver.cpp
 *
 * Implements an I2C slave device that is sent command data from the
 * master microcontroller that is interpreting IR remote input.
 *
 * Used as a stand-in for the diorama's central control microcontroller.
 *
 * This code is placed in the public domain. Used libraries may have
 * other licenses.
 */

#include <Arduino.h>

#include <Wire.h>


// Command just read
byte  command = 0;
// Parameter just read
byte  param = 0;

#define CMD_SIZE 2

#define IRDATA_AVAILABLE 2 // Correct amount of data is available
#define IRDATA_READING 1   // Currently reading data - all of it not yet available
#define IRDATA_WAITING 0   // Currently waiting for data
#define IRDATA_ERROR 0xff  // Wrong amount of data read

// Defines whether I2C reader state
byte state = IRDATA_WAITING;


// Executes whenever command data is received from master
void receiveCmd(int byteCount) {
  if (byteCount != CMD_SIZE) {
    state = IRDATA_ERROR;
  }
  else {
    state = IRDATA_READING;
    command = Wire.read();
    param = Wire.read();
    state = IRDATA_AVAILABLE;
  }
}

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveCmd);   // register i2c receive event handler
  Serial.begin(9600);           // start serial for output
  Serial.println("Mock command reader running");
  Serial.println();
}

void loop()
{
  delay(100);
  switch(state) {
    case IRDATA_AVAILABLE:
      Serial.println();
      Serial.println(F("DATA RECEIVED:"));
      Serial.print(F("  Command: "));
      Serial.println(command, HEX);
      Serial.print(F("  Param: "));
      Serial.println(param, HEX);
      state = IRDATA_WAITING;
      break;
    case IRDATA_READING:
      Serial.println();
      Serial.println(F("Receiving data"));
      break;
    case IRDATA_ERROR:
      Serial.println();
      Serial.println(F("ERROR: Bad data count"));
      break;
    case IRDATA_WAITING:
      Serial.print(F("."));
      break;
    default:
      Serial.println();
      Serial.println(F("*** Invalid IRDATA_XXX value"));
  }
}
