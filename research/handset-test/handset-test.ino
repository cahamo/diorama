/*
 * Detects and interprets codes emitted by the actual infra-red handset that is being used
 * with the diorama. Reports the name of the key being pressed as it appears on the handset
 * and flags if the key is repeating. Turns on a LED whenever a signal is received from the
 * handset.
 *
 * Results are sent over the serial port.
 * 
 * Copyright (c) 2021, Peter Johnson (cahamo.github.io)
 * MIT License
 */
 
#include <Arduino.h>

/*
 * Define macros for input and output pin etc.
 */
#define DECODE_NEC                // We're using an NEC compatible handset
#define NO_LEGACY_COMPATIBILITY   // Don't need legacy support for NEC
#define EXCLUDE_EXOTIC_PROTOCOLS  // NEC is not "exotic"!

#include <IRremote.h>

#include "handset-test.h"

#define IRSensorPin 2
#define FeedbackLEDPin 3

void reportKey(String s) {
  Serial.print("Key pressed: ");
  Serial.print(s);
}

void setup() {
  Serial.begin(115200);
  // Display running program & lib version
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing IRemote library version " VERSION_IRREMOTE));
  
  IrReceiver.begin(IRSensorPin, ENABLE_LED_FEEDBACK, FeedbackLEDPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (IrReceiver.decode()) {
    IrReceiver.resume(); // Resume listening to sensor while current readings are processed

    // Check if this is an NEC handset - assume it's the one we're using for diorama
    if (IrReceiver.decodedIRData.protocol == NEC) {

      // Interpret which key was pressed
      switch(IrReceiver.decodedIRData.command) {
        case REMOTE_KEY_0: 
          reportKey("0");
          break;
        case REMOTE_KEY_1: 
          reportKey("1");
          break;
        case REMOTE_KEY_2: 
          reportKey("2");
          break;
        case REMOTE_KEY_3: 
          reportKey("3");
          break;
        case REMOTE_KEY_4: 
          reportKey("4");
          break;
        case REMOTE_KEY_5: 
          reportKey("5");
          break;
        case REMOTE_KEY_6: 
          reportKey("6");
          break;
        case REMOTE_KEY_7: 
          reportKey("7");
          break;
        case REMOTE_KEY_8: 
          reportKey("8");
          break;
        case REMOTE_KEY_9: 
          reportKey("9");
          break;
        case REMOTE_KEY_STAR:
          reportKey("*");
          break;
        case REMOTE_KEY_HASH:
          reportKey("#");
          break;
        case REMOTE_KEY_UP:
          reportKey("Up-Arrow");
          break;
        case REMOTE_KEY_DOWN:
          reportKey("Down-Arrow");
          break;
        case REMOTE_KEY_LEFT:
          reportKey("Left-Arrow");
          break;
        case REMOTE_KEY_RIGHT:
          reportKey("Right-Arrow");
          break;
        case REMOTE_KEY_OK:
          reportKey("OK");
          break;
        default:
          reportKey("@@@ Unknown key press");
          break;
      }
      
      // Check if key is repeating
      if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        Serial.println(" - repeat");
      }
      else {
        Serial.println();
      }
    }
    else
      // Dunno what this is, but it's not our handset
      Serial.println("@@@ Unknown protocol");  
  }
}
