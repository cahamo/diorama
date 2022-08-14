# About this `include` directory

This directory contains private, project specific libraries.

PlatformIO compiles them to static libraries and links them into the executable
file. For information about how PlatformIO is configured to find these files see the [PlatformIO Library Dependency Finder documentation](https://docs.platformio.org/page/librarymanager/ldf.html).

The source code of each library is placed into a separate directory, as follows:

```plaintext
|--lib
|  |
|  |--CountdownTimer
|  |  |- CountdownTimer.cpp
|  |  |- CountdownTimer.h
|  |
|  |--RGBLED
|  |  |- RGBLED.cpp
|  |  |- RGBLED.h
|  |
|  |- README.md --> THIS FILE
|
|- platformio.ini
|--src
  |- <<main program source files>>
```

The libraries are included in the main program source files like this:

```c++
#include <CountdownTimer.h>
#include <RGBLED.h>
```
