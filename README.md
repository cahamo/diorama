# diorama

A scenic diorama project with electronic control.

This repository contains the project's source code. A description of the project along with details of progress can be found on the [Diorama](https://cahamo.github.io/diorama/) page of the [Cahamo](https://cahamo.github.io/) website.

## Contents

At present the only code is in the `research` sub-directory - it is code used with experimental circuits. The `research` sub-directory is further divided into several sub-directories, each containing related projects. They are:

* `ir-control-tests` - a set of projects used to investigate the operation of the infra-red controller and receiver that is to be used to control the diorama's electronic features, along with associated components. [[More information](./research/ir-control-tests/README.md)].

* `led-mux-tests` - a set of programs used to test the effectiveness of using software serial libraries to communicate between microcontrollers. [[More information](./research/led-mux-tests/README.md)].

* `serial-tests` - a set of programs used to test the effectiveness of using serial communications between microcontrollers. [[More information](/research/serial-tests/README.md)].

* `i2c-tests` - a set of programs used to test the effectiveness of using I2C for communication between microcontrollers. [[More information](/research/i2c-tests/README.md)].

* `feature-tests` - programs used to test the diorama's electro-mechanical features. [[More information](/research/feature-tests/README.md)].
