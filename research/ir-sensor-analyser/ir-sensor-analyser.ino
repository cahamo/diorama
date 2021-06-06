/*
 * Detects and interprets codes emitted by an infra-red control handsets and sends
 * information about them over the serial port.
 * 
 * Based closely on the "SimpleReciever" example code provided with the IRremote library v3.x at
 * https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 * MIT License
 */
 
#include <Arduino.h>
#include <IRremote.h>

#define IRSensorPin 2
#define FeedbackLEDPin 3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Write out running program
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing IRemote library version " VERSION_IRREMOTE));
  // Initialise receiver
  IrReceiver.begin(IRSensorPin, ENABLE_LED_FEEDBACK, FeedbackLEDPin);

}

void loop() {
  if (IrReceiver.decode()) {
    // Print a short summary of received data
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume(); // Enable receiving of the next value
  }
}
