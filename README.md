# GNSS-Parser for ESP32

## Table of Contents

- [GNSS-Parser for ESP32](#gnss-parser-for-esp32)
  - [Table of Contents](#table-of-contents)
  - [About](#about)
  - [Use with PlatformIO](#use-with-platformio)
  - [Use with esp-idf](#use-with-esp-idf)
  - [Maintainer](#maintainer)

## About

Improved version of the ESP32 [nmea0183_parser](https://github.com/espressif/esp-idf/blob/8464186e67e34b417621df6b6f1f289a6c60b859/examples/peripherals/uart/nmea0183_parser) from Espressif.

## Use with PlatformIO

- Add the library to the `lib_deps` parameter

```sh
lib_deps =
    https://gitlab.server-kampert.de/esp32/libraries/GNSS-parser.git
```

- Run `pio -t menuconfig` from the root of your project to configure the driver

## Use with esp-idf

- Create a directory `components` in your project root
- Clone the repository into the `components` directory
- Build the project

## Maintainer

- [Daniel Kampert](mailto:daniel.kameprt@kampis-elektroecke.de)
