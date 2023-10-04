
#include <Arduino.h>

/*
 * Define macros for input and output pin etc.
 */
#define DECODE_NEC                // We're using an NEC compatible handset
#define NO_LEGACY_COMPATIBILITY   // Don't need legacy support for NEC
#define EXCLUDE_EXOTIC_PROTOCOLS  // NEC is not "exotic"!
#include <IRremote.h>

#include <Wire.h>

#include "../../common/commands.h"
#include "../../common/i2c.h"
#include "ir-handset-codes.h"

// Private libraries
#include <RGBLED.h>
#include <CountdownTimer.h>

#define DEBUG
#include "../../common/debug.h"

// LED feedback pin
#define IR_SENSOR_PIN 2
#define FEEDBACK_RED_PIN 9
#define FEEDBACK_GREEN_PIN 10
#define FEEDBACK_BLUE_PIN 11

// Create object to manage RGB feedback LED
RGBLED feedbackLED(FEEDBACK_RED_PIN, FEEDBACK_GREEN_PIN, FEEDBACK_BLUE_PIN);

// Create countdown object to manage command wait time-outs
CountdownTimer cmdTimeout(2500);  // 2.5 seconds

// Command reader state machine state
#define STATE_ROOT 0
#define STATE_LIGHTING_SECTION 1
#define STATE_LIGHTING_ALL 2
#define STATE_FEATURE 3
#define STATE_PROGRAM 4

// Feedback LED flash / blink times & flash counts
// flashing
#define FEEDBACK_LED_FLASH_OFF_TIME 50
#define FEEDBACK_LED_FLASH_ON_TIME 150
#define FEEDBACK_LED_FLASH_COUNT 4
// blinking
#define FEEDBACK_LED_BLINK_OFF_TIME 100
#define FEEDBACK_LED_BLINK_ON_TIME_SHORT 50
#define FEEDBACK_LED_BLINK_ON_TIME_LONG 250
#define FEEDBACK_LED_BLINK_COUNT 1

// Long and short LED blink flags
#define FEEDBACK_LED_BLINK_SHORT false
#define FEEDBACK_LED_BLINK_LONG true

// Global variables
uint8_t state = STATE_ROOT;

/*!
 * @brief Maps a IR handset code onto an associated ID number.
 *
 * Handset digits 1..9 map onto zero based IDs 0..8.
 * If permitSpecial flag is true buttons representing flicker lights map to their IDs.
 * The meaning of the ID depends on the context the function is called, except for
 * the keys representing flicker lights.
 *
 * @param code Handset code
 * @param permitSpecial Whether "special" key codes are permitted
 * @return Returns required ID or LIGHTING_INVALID_LIGHT_ID if code not recognised
*/
uint8_t mapIrKeyToID(uint8_t code, bool permitSpecial = false) {
  switch(code) {
    case REMOTE_KEY_1: return 0;
    case REMOTE_KEY_2: return 1;
    case REMOTE_KEY_3: return 2;
    case REMOTE_KEY_4: return 3;
    case REMOTE_KEY_5: return 4;
    case REMOTE_KEY_6: return 5;
    case REMOTE_KEY_7: return 6;
    case REMOTE_KEY_8: return 7;
    case REMOTE_KEY_9: return 8;
    case REMOTE_KEY_HASH: return permitSpecial ? LIGHTING_FLICKER_LIGHT_ID : LIGHTING_INVALID_LIGHT_ID;
    default: return LIGHTING_INVALID_LIGHT_ID;
  }
}

bool isValidSectionKey(uint8_t keyCode) {
  uint8_t id = mapIrKeyToID(keyCode);
  return (id < LIGHTING_SECTION_COUNT);
}

bool isValidLightKeyWithinSection(uint8_t sectionId, uint8_t lightKeyCode) {
  uint8_t lightId = mapIrKeyToID(lightKeyCode, true);
  if (sectionId >= LIGHTING_SECTION_COUNT)
    return false;
  if (lightId < lightingNormalLightsInSection[sectionId])
    return true;
  if ((lightId == LIGHTING_FLICKER_LIGHT_ID)
    && lightingHasFlickerLightInSection[sectionId])
    return true;
  return false;
}

bool isValidFeatureKey(uint8_t keyCode) {
  uint8_t id = mapIrKeyToID(keyCode);
  return (id < FEATURE_COUNT);
}

bool isValidProgramKey(uint8_t keyCode) {
  uint8_t id = mapIrKeyToID(keyCode);
  return (id < PROGRAM_COUNT);
}

/*!
 * @brief Log given command details to Serial, iff DEBUG is defined.
 *
 * @param mainCmd ID of main command in range 0x1..0xf
 * @param subCmd ID of sub-command in range 0x1..0xf or 0x0 if no sub-command
 * @param param Parameter data. Format is command dependent. Defaults to CMDPARAM_NULL (0)
 */
void logCommand(uint8_t mainCmd, uint8_t subCmd, uint8_t param){
  DBGprint(F("Command: "));
  DBGprintfmt(mainCmd, HEX);
  DBGprint(F("  Sub-command: "));
  DBGprintfmt(subCmd, HEX);
  DBGprint(F("  Param: "));
  DBGprintlnfmt(param, HEX);
  DBGblankln();
}

/*!
 * @brief Send given command, sub-command and parameter bytes over I2C to central control microcontroller.
 *
 * @param mainCmd ID of main command in range 0x1..0xf
 * @param subCmd ID of sub-command in range 0x1..0xf or 0x0 if no sub-command
 * @param param Parameter data. Format is command dependent. Defaults to CMDPARAM_NULL (0)
 */
void sendCommand(uint8_t mainCmd, uint8_t subCmd, uint8_t param) {
  Wire.beginTransmission(CENTRAL_CONTROL_I2C_ADDR);
  Wire.write((byte)MakeCommand(mainCmd, subCmd));
  Wire.write(param);
  Wire.endTransmission();
}

void flashFeedbackLED(RGBColour colour) {
  feedbackLED.flash(
    colour,
    FEEDBACK_LED_FLASH_COUNT,
    FEEDBACK_LED_FLASH_OFF_TIME,
    FEEDBACK_LED_FLASH_ON_TIME
  );
}

void blinkFeedbackLED(RGBColour colour, bool longFlash) {
  feedbackLED.blink(
    colour,
    FEEDBACK_LED_BLINK_OFF_TIME,
    longFlash ? FEEDBACK_LED_BLINK_ON_TIME_LONG : FEEDBACK_LED_BLINK_ON_TIME_SHORT,
    false
  );
}

void reportTimeout() {
  DBGprintln(F("--- Timeout"));
  DBGblankln();
  flashFeedbackLED(RGBLED::Amber);
}

void reportSuccess() {
  DBGprintln(F("!!! Success"));
  blinkFeedbackLED(RGBLED::Green, FEEDBACK_LED_BLINK_LONG);
}

void reportError() {
  DBGprintln(F("*** Error"));
  DBGblankln();
  flashFeedbackLED(RGBLED::Red);
}

void requestFurtherInput() {
  DBGprintln(F("%%% Further input requested"));
  DBGblankln();
  feedbackLED.display(RGBLED::Amber);
  cmdTimeout.start();
}

void handleSuccess(uint8_t mainCmd, uint8_t subCmd, uint8_t param = CMDPARAM_NULL) {
  reportSuccess();
  logCommand(mainCmd, subCmd, param);
  sendCommand(mainCmd, subCmd, param);
}

void processKey(uint8_t keyCode) {
  static uint8_t sectionId = 0xff;

  switch (state) {
    case STATE_ROOT:
      DBGprintln(F("Processing ROOT state"));
      if (isValidSectionKey(keyCode)) {
        sectionId = mapIrKeyToID(keyCode);
        state = STATE_LIGHTING_SECTION;
        DBGprint(F("Lighting: section-light: "));
        DBGprintfmt(sectionId, HEX);
        DBGprintln(F("/???"));
        requestFurtherInput();
      }
      else {
        switch (keyCode) {

          case REMOTE_KEY_0:
            state = STATE_LIGHTING_ALL;
            DBGprintln(F("Lighting: all: ???"));
            requestFurtherInput();
            break;

          case REMOTE_KEY_HASH:
            state = STATE_FEATURE;
            DBGprintln(F("Feature: ???: ???"));
            requestFurtherInput();
            break;

          case REMOTE_KEY_STAR:
            state = STATE_PROGRAM;
            DBGprintln(F("Program: ???: ???"));
            requestFurtherInput();
            break;

          case REMOTE_KEY_UP:
            DBGprintln(F("Ambient: brightness: brighten"));
            handleSuccess(CMD_AMBIENT, SUBCMD_AMBIENT_BRIGHTNESS, CMDPARAM_AMBIENT_BRIGHTNESS_UP);
            break;

          case REMOTE_KEY_DOWN:
            DBGprintln(F("Ambient: brightness: dim"));
            handleSuccess(CMD_AMBIENT, SUBCMD_AMBIENT_BRIGHTNESS, CMDPARAM_AMBIENT_BRIGHTNESS_DOWN);
            break;

          case REMOTE_KEY_LEFT:
            DBGprintln(F("Ambient: switch: off"));
            handleSuccess(CMD_AMBIENT, SUBCMD_AMBIENT_SWITCH, CMDPARAM_AMBIENT_SWITCH_OFF);
            break;

          case REMOTE_KEY_RIGHT:
            DBGprintln(F("Ambient: switch: on"));
            handleSuccess(CMD_AMBIENT, SUBCMD_AMBIENT_SWITCH, CMDPARAM_AMBIENT_SWITCH_ON);
            break;

          case REMOTE_KEY_OK:
            DBGprintln(F("Reset: <null>: <null>"));
            handleSuccess(CMD_RESET, SUBCMD_NULL);
            break;

          default:
            DBGprintln(F("**Unknown Command**"));
            reportError();
            break;
        }
      }
      break;

    case STATE_LIGHTING_SECTION:

      DBGprintln(F("Processing LIGHTING SECTION state"));

      if (keyCode == REMOTE_KEY_0)  {
        DBGprintln(F("Lighting: section-all: "));
        DBGprintlnfmt(sectionId, HEX);
        handleSuccess(CMD_LIGHTING, SUBCMD_LIGHTING_SECTION, sectionId);
      }

      else if (isValidLightKeyWithinSection(sectionId, keyCode)) {
        uint8_t lightId = mapIrKeyToID(keyCode, true);
        DBGprint(F("Lighting: section-light: "));
        DBGprintfmt(sectionId, HEX);
        DBGprint(F("/"));
        DBGprintlnfmt(lightId, HEX);
        handleSuccess(CMD_LIGHTING, SUBCMD_LIGHTING_LIGHT, MakeSectionLightParam(sectionId, lightId));
      }

      else {
        DBGprintln(F("Lighting: **ERROR**"));
        reportError();
      }

      state = STATE_ROOT;
      break;

    case STATE_LIGHTING_ALL:

      DBGprintln(F("Processing LIGHTING_ALL state"));
      switch(keyCode) {

        case REMOTE_KEY_OK:
        case REMOTE_KEY_0:
          DBGprintln(F("Lighting: all: everything"));
          handleSuccess(CMD_LIGHTING, SUBCMD_LIGHTING_ALL, CMDPARAM_LIGHTING_ALL_EVERYTHING);
          break;

        case REMOTE_KEY_STAR:
          DBGprintln(F("Lighting: all: normal"));
          handleSuccess(CMD_LIGHTING, SUBCMD_LIGHTING_ALL, CMDPARAM_LIGHTING_ALL_NORMAL);
          break;

        case REMOTE_KEY_HASH:
          DBGprintln(F("Lighting: all: flicker"));
          handleSuccess(CMD_LIGHTING, SUBCMD_LIGHTING_ALL, CMDPARAM_LIGHTING_ALL_FLICKER);
          break;

        default:
          DBGprintln(F("Lighting: all: **Error**"));
          reportError();
      }

      state = STATE_ROOT;
      break;

    case STATE_FEATURE:
      DBGprintln(F("Processing FEATURE state"));
      if (isValidFeatureKey(keyCode)) {
        uint8_t featureId = mapIrKeyToID(keyCode);
        DBGprint(F("Feature: activate: "));
        DBGprintlnfmt(featureId, HEX);
        handleSuccess(CMD_FEATURE, SUBCMD_FEATURE_ACTIVATE, featureId);
      }
      else if (keyCode == REMOTE_KEY_0) {
        DBGprintln(F("Feature: reset: <null>"));
        handleSuccess(CMD_FEATURE, SUBCMD_FEATURE_RESET);
      }
      else {
        DBGprintln(F("Feature: **ERROR**"));
        reportError();
      }
      state = STATE_ROOT;
      break;

    case STATE_PROGRAM:
      DBGprintln(F("Processing PROGRAM state"));
      if (isValidProgramKey(keyCode)) {
        uint8_t programId = mapIrKeyToID(keyCode);
        DBGprint(F("Program: run: "));
        DBGprintlnfmt(programId, HEX);
        handleSuccess(CMD_PROGRAM, SUBCMD_PROGRAM_RUN, programId);
      }
      else if (keyCode == REMOTE_KEY_0) {
        DBGprintln(F("Program: stop: <null>"));
        handleSuccess(CMD_PROGRAM, SUBCMD_PROGRAM_STOP);
      }
      else {
        DBGprintln(F("Program: **ERROR**"));
        reportError();
      }
      state = STATE_ROOT;
      break;

    default:
      DBGprintln(F("Unknown state encountered"));
      reportError();
      state = STATE_ROOT;
      break;
  }
}

void setup() {
  feedbackLED.display(RGBLED::Blue);

  DBGbegin(9600);
  DBGprintln(F("IR Command Processor Running..."));
  DBGblankln();

  IrReceiver.begin(IR_SENSOR_PIN, false, 0);

  // Join I2C bus as master
  Wire.begin();
  DBGprintln("Intialised Wire library");

  DBGprint(F("Timeout length: "));
  DBGprintln(cmdTimeout.getLengthMs());
  DBGprint(F("Is countdown running: "));
  DBGprintln(cmdTimeout.isRunning() ? "YES" : "NO");

  feedbackLED.display(RGBLED::Off);
}

void loop() {

  if (cmdTimeout.isRunning()) {
    DBGprint(F("Timeout remaining: "));
    DBGprint(cmdTimeout.remainingTimeMs());
    DBGprint(F(" & ellapsed: "));
    DBGprintln(cmdTimeout.ellapsedTimeMs());
  }

  // Process any key timeout
  if (cmdTimeout.hasTimedOut()) {
    cmdTimeout.stop();
    reportTimeout();
    state = STATE_ROOT;
  }

  // Check if anything has happened since last loop
  if (! IrReceiver.decode()) return;

  // Resume listening to sensor while current readings are processed
  IrReceiver.resume();

  // Bail out if this isn't an NEC handset (it should be, but better check!)
  if (IrReceiver.decodedIRData.protocol != NEC) {
    if (cmdTimeout.isRunning()) {
      cmdTimeout.restart();
    }
    return;
  }

  // Ignore key repeats
  if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) return;

  cmdTimeout.stop();

  // Flash LED to acknowledge
  blinkFeedbackLED(RGBLED::Blue, FEEDBACK_LED_BLINK_SHORT);

  DBGprint(F("Key code: "));
  DBGprintlnfmt(IrReceiver.decodedIRData.command, HEX);

  processKey(IrReceiver.decodedIRData.command);

  delay(10);
}
