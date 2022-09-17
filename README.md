# GRC Dash

## About

An electronic dash for the different GRC vehicles 

## Installing & Building with Conan
Run the conan install script (`-pr` is the profile being used, the options are found in [ConanProfiles](./ConanProfiles/))
```bash
conan install . -if build -pr:h=ConanProfiles/Visual-Studio-2019-Release -pr:b=ConanProfiles/Visual-Studio-2019-Release --build=missing
```

Run the conan build script
```bash
conan build -if build .
```

Final executables will be found in the build/bin folder.

## Code Standards

Naming standards are as follows:
```C++
namespace PascalCase {}
class PascalCase {};
int camelCase() {};
int snake_case = 0;
```

Formatting is handled by clang format.

## Resources

* [<linux/can.h>](https://github.com/torvalds/linux/blob/master/include/uapi/linux/can.h)
* [SocketCAN Intro](https://copperhilltech.com/what-is-socketcan/)
* [PiCAN1](https://copperhilltech.com/pican-with-gps-gyro-accelerometer-can-bus-board-for-raspberry-pi/)/[PiCAN2](https://copperhilltech.com/pican-2-can-bus-interface-for-raspberry-pi/)
* [Raspberry Pi w/ CanBus](https://www.hackster.io/youness/how-to-connect-raspberry-pi-to-can-bus-b60235)
* Not used, but good cross reference for the SocketCAN intro: [socketcan-cpp](https://github.com/siposcsaba89/socketcan-cpp)