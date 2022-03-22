# I2C Tests

This is a set of programs used to test the effectiveness of using I2C to communicate between microcontrollers.

I2C has been identified as a possible alternative to using serial for the microcontroller running the diorama's IR subsystem to send data to the main microcontroller.

The programs were all developed using PlatformIO. Details follow.

## `i2c-master-write-attiny85`

Sets up an ATtiny85 as an I2C master device, using the `TinyWireM` library to provide the I2C master device functionality.

The program sends text data every 500ms to a slave device with address 0x04.

_Used in diorama experiment [#5](https://cahamo.github.io/projects/diorama/experiment-5)_

## `i2c-master-write-nano`

Sets up an Arduino Nano as an I2C master device, using the `Wire` library to provide full I2C support.

The program sends text data every 500ms to a slave device with address 0x04.

_Used in diorama experiment [#5](https://cahamo.github.io/projects/diorama/experiment-5)_

## `i2c-slave-read-uno`

Sets up an Arduino Uno as an I2C slave device with address 0x04, using the `Wire` library to provide full I2C support. Data received is written out to the Uno's built in USB serial port.

The program is used to debug the data sent by an ATtiny85 or  Arduino Nano running `i2c-master-write-attiny85` or `i2c-master-write-nano` respecyively.

_Used in diorama experiment [#5](https://cahamo.github.io/projects/diorama/experiment-5)_

