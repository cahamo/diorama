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

#include "../../../../src/common/commands.h"


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

void writeCmdDetails()
{
  byte mainCmd = ExtractMainCommand(command);
  byte subCmd = ExtractSubCommand(command);
  Serial.print(F("Command: "));
  switch(mainCmd) {
    case CMD_LIGHTING:
      Serial.println(F("MODEL LIGHTING"));
      Serial.print(F("  Sub-Command: "));
      switch(subCmd) {
        case SUBCMD_LIGHTING_LIGHT: {
          Serial.println(F("Toggle Specified Light"));
          byte lightID = ExtractLightParam(param);
          byte sectionID = ExtractSectionParam(param);
          Serial.print(F("  Param: Section "));
          Serial.print(sectionID);
          Serial.print(F(", Light "));
          Serial.println(lightID);
         }
          break;
        case SUBCMD_LIGHTING_SECTION:
          Serial.println(F("Toggle Secton Lights"));
          Serial.print(F("  Param: Section "));
          Serial.println(param);
          break;
        case SUBCMD_LIGHTING_ALL:
          Serial.println(F("Toggle All"));
          Serial.print(F("  Param: "));
          switch(param) {
            case CMDPARAM_LIGHTING_ALL_EVERYTHING:
              Serial.println(F("Everything "));
              break;
            case CMDPARAM_LIGHTING_ALL_NORMAL:
              Serial.println(F("Normal "));
              break;
            case CMDPARAM_LIGHTING_ALL_FLICKER:
              Serial.println(F("Flicker "));
              break;
            default:
              Serial.println(F("** Unknown "));
              break;
          }
          break;
        default:
          Serial.println(F("** Unknown"));
          break;
      }
      break;
    case CMD_FEATURE:
      Serial.println(F("FEATURE"));
      Serial.print(F("  Sub-Command: "));
      switch(subCmd) {
        case SUBCMD_FEATURE_ACTIVATE:
          Serial.println(F("Activate or Toggle"));
          Serial.print(F("  Param: Feature ID "));
          Serial.println(param);
          break;
        case SUBCMD_FEATURE_RESET:
          Serial.println(F("Reset All"));
          break;
        default:
          Serial.println(F("** Unknown"));
          break;
      }
      break;
    case CMD_AMBIENT:
      Serial.println(F("AMBIENT LIGHTING"));
      Serial.print(F("  Sub-Command: "));
      switch(subCmd) {
        case SUBCMD_AMBIENT_SWITCH:
          Serial.println(F("Switch"));
          Serial.print(F("  Param: "));
          switch(param) {
            case CMDPARAM_AMBIENT_SWITCH_OFF:
              Serial.println(F("Off"));
              break;
            case CMDPARAM_AMBIENT_SWITCH_ON:
              Serial.println(F("On"));
              break;
            default:
              Serial.println(F("** Unknown"));
              break;
          }
          break;
        case SUBCMD_AMBIENT_BRIGHTNESS:
          Serial.println(F("Brightness"));
          Serial.print(F("  Param: "));
          switch(param) {
            case CMDPARAM_AMBIENT_BRIGHTNESS_UP:
              Serial.println(F("Up"));
              break;
            case CMDPARAM_AMBIENT_BRIGHTNESS_DOWN:
              Serial.println(F("Down"));
              break;
            default:
              Serial.println(F("** Unknown"));
              break;
          }
          break;
        default:
          Serial.println(F("** Unknown"));
          break;
      }
      break;
    case CMD_PROGRAM:
      Serial.println(F("PROGRAM"));
      Serial.print(F("  Sub-Command: "));
      switch(subCmd) {
        case SUBCMD_PROGRAM_RUN:
          Serial.println(F("Run"));
          Serial.print(F("  Param: Program ID "));
          Serial.println(param);
          break;
        case SUBCMD_PROGRAM_STOP:
          Serial.println(F("Stop Current"));
          break;
        default:
          Serial.println(F("** Unknown"));
          break;
      }
      break;
    case CMD_RESET:
      Serial.println(F("RESET"));
      break;
    default:
      Serial.println(F("** UNKNOWN"));
  }
}

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveCmd);   // register i2c receive event handler
  Serial.begin(9600);           // start serial for output
  Serial.println("Mock command interpreter running");
  Serial.println();
}

void loop()
{
  delay(10);
  switch(state) {
    case IRDATA_AVAILABLE:
      Serial.println();
      writeCmdDetails();
      state = IRDATA_WAITING;
      break;
    case IRDATA_READING:
      Serial.println();
      Serial.println(F("Receiving data"));
      break;
    case IRDATA_ERROR:
      Serial.println();
      Serial.println(F("ERROR: Bad data count"));
      state = IRDATA_WAITING;
      break;
    case IRDATA_WAITING:
      // Do nothing
      break;
    default:
      Serial.println();
      Serial.println(F("*** Invalid IRDATA_XXX value"));
  }
}
