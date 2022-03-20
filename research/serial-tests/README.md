# SoftwareSerial Tests

This is a pair of programs used to test the effectiveness of using the `SoftwareSerial` library to communicate between microcontrollers.

One possible way for the microcontroller running the diorama's IR subsystem to send data to the main microcontroller is to use this method.

The programs are:

* `nano-serial-test` - this program is designed to run on an Arduino Nano. It flashes the built in LED and transmits the state of the LED over serial.
* `copy-softserial-to-serial-uno` - this program receives data using `SoftwareSerial` and copies it to its `Serial` port for display in a serial monitor.

> **NOTE:** A similar test was tried using an ATtiny85 microcontroller as a serial transmitter, but this failed and garbage was received by the receiving microcontroler. This could be down to a timing issue on the ATtiny85.
