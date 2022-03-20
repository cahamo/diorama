# Serial Tests

This is a set of programs used to test the effectiveness of using software serial libraries to communicate between microcontrollers.

One possible way for the microcontroller running the diorama's IR subsystem to send data to the main microcontroller is to use this method.

The programs were all developed using PlatformIO. Details follow.

## `nano-serial-test`

This program is designed to run on an Arduino Nano. It flashes the built in LED and transmits the state of the LED over serial using the `SoftwareSerial` library.

## `attiny-serial-test`

This program is designed to run on an ATtiny85. It flashes a LED connected to pin PB1 and transmits the state of the LED over serial using the `ATtinySerialOut` library.

> **NOTE:** While this program compiles and runs the transmitted data (captured using `copy-softserial-to-serial-uno`) was corrupt. Changing from the `ATtinySerialOut` library to `SoftwareSerial` made no difference. It seems that there are known timing issues with ATtinys while transmitting over serial. Some fixes were tried but didn't work. Note though that the similar `nano-serial-test` did work.

## `copy-softserial-to-serial-uno`

This program receives data using the `SoftwareSerial` library and copies it to its `Serial` port for display in a serial monitor.
