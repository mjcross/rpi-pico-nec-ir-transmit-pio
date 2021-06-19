# rpi-pico-nec-ir-transmit-pio
A library and simple demo to transmit 'NEC' IR control codes using the PIO state machines on the Raspberry Pi PICO / RP2040, written in 'C' and PIO assembler.

## Overview
The library uses two of the eight state machines (SM) on the RP2040: one to generate bursts of 38kHz carrier and the other to construct and send frames of data.
All the user's application has to do is push 32-bit words to the SM FIFO either directly or via API functions. These allow you to send normal (8 bit address and
data), extended (16 bit address and data) or raw (32 bit data) frames.

The implementation of the 'NEC' protocol is based on the description [here](https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol).

## How to build
The repository is structured as a `CMake` project based on the RPI PICO C/C++ SDK so you will need to 
[install and configure those tools](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf) first.

1. Download a copy of the repository to your machine (for example by clicking on `Code -> Download ZIP` above.
2. Create a build directory at the top level

```
$ cd <project-directory>
$ mkdir build
```

3. Configure the build system and build the project

```
$ cd build
$ cmake ..
$ make
```

4. After a successful build the binary for the simple demo application can be found at `build/demo/nec_transmit_demo.uf2` (or `.elf`) which can be uploaded to
a device in the usual way.

_You can of course fine tune the above if you use a different build system or IDE._

## API functions
To be written - in the meantime please have a look at the comments in `demo/demo.c` which are pretty self-explanatory.

## Using the library in your own project
To be written - in the meantime please have a look at the `CMakeLists.txt` files in the different directories.
