# IR Control Tests

This is a set of projects used to investigate the operation of the infra-red controller and receiver that is to be used to control the diorama's electronic features.

All projects were developed under PlatformIO. The projects are listed below.

## `ir-sensor-analyser`

Contains C++ code `ir-sensor-analyser.cpp`, a sketch used to detect the type of an infra red remote handset and the code generated by each key.

Uses the [IRemote library v3](https://github.com/Arduino-IRremote/Arduino-IRremote). Based closely on that library's "SimpleReceiver" example code.

This sketch was used to find the codes emitted by the handset that is to be used to control the diorama's features.

_Used in diorama experiment [#1](https://cahamo.github.io/diorama/experiment-1)_

## `handset-test`

Contains C++ code file `handset-test.cpp` and header file `handset-test.h`. The sketch uses the information gleaned from the `ir-sensor-analyser` sketch to detects and interpret codes emitted by the infra-red handset that is being used with the diorama.

Reports the name of the key being pressed on the Serial port and flags if the key is repeating. Also turns on a LED whenever a signal is received from the handset.

Uses the [IRemote library v3](https://github.com/Arduino-IRremote/Arduino-IRremote).

_Used in diorama experiment [#1](https://cahamo.github.io/projects/diorama/experiment-1)_

## `rgb-led-colour-balance`

Contains C++ code file `rgb-led-colour-balance.cpp`, a program used to display a repeating sequence of various colours in an RGB LED.

The purpose of the program is to enable different values of resistors to be tried to balance the intensity of the RGB LED's red, green and blue elements so that the results can be seen straight away.

No additional libraries are required.

_Used in diorama experiment [#6](https://cahamo.github.io/projects/diorama/experiment-6)_

## `mock-i2c-command-receiver`

Contains C++ code file `mock-i2c-command-receiver.cpp`, that implements an I2C slave device that is sent command data from the master microcontroller that is interpreting IR remote input.

The purpose of the program is to be used as a stand-in for the diorama's central control microcontroller when testing the the IR remote controller code.

Uses the Arduino Wire library.

_Not used in any diorama experiments. It is designed to help test production code._
