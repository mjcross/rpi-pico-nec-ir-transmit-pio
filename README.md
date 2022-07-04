# rpi-pico-nec-ir-transmit-pio
A library and simple demo to transmit 'NEC' IR control codes using the PIO state machines on the Raspberry Pi PICO / RP2040, written in 'C' and PIO assembler.

## NOTE
This code (and the corresponding *receive* library) have now been incorporated into the RPi official repository, so this repository is no longer actively maintained. Please use [https://github.com/raspberrypi/pico-examples/tree/master/pio/ir_nec](https://github.com/raspberrypi/pico-examples/tree/master/pio/ir_nec) instead.

## Overview
The library uses two of the eight state machines (SM) on the RP2040: one to generate bursts of 38kHz carrier and the other to construct and send frames of data.
All the user's application has to do is push 32-bit words to the SM FIFO either directly or via API functions. These allow you to send normal (8 bit address and
data), extended (16 bit address and data) or raw (32 bit data) frames.

The implementation of the 'NEC' protocol is based on the description [here](https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol).

To transmit the signal you will need to connect an infra-red LED between your chosen GPIO pin and ground via a suitable current limiting resistor (e.g. 1.5k).

## How to build
The repository is structured as a `CMake` project based on the RPI PICO C/C++ SDK so you will need to 
[install and configure those tools](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf) first.

Once the tools are set up:

1. Download a copy of the repository to your machine (for example by clicking on the green `Code` button above and selecting `Download ZIP`.
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

4. After a successful build the binary for the simple demo application can be found at `build/demo/nec_transmit_demo.uf2` (or `.elf`). This can be uploaded to
a device in the usual way.

_You can of course fine tune the above if you use a different build system or IDE._

## API documentation
The API functions are in `nec_transmit/nec_transmit.c`. You can find some notes on usage in `demo/demo.c`.

### int nec_tx_init(PIO pio, uint pin_num)
Initialise a PIO to transmit NEC IR frames on the specified GPIO pin.
Returns state machine number on success, otherwise -1 if an error occurred.

This function must be called once before the other functions are used. It uploads the binaries to the first two unused state machines on the specified PIO,
and returns the index of the one that is listening on the FIFO.

### void nec_tx(PIO pio, int sm, uint8_t address, uint8_t data)
Send a normal 'NEC' IR frame with 8-bit address and data as used by various types of consumer remote control.
The `pio` and `sm` should match those passed to and returned by `nec_tx_init()`.
Note that the data bits are sent LSB first.
The function returns immediately unless the SM FIFO is full; however most receivers need you to pause at least 100ms before sending another frame.

### void nec_tx_extended(PIO pio, int sm, uint16_t address, uint16_t data)
Send an extended 'NEC' IR frame with 16-bit address and data.
_Note: to avoid confusing non-extended mode devices in the vicinity you should not send frames where the upper and lower address bytes are each other's inverse._
The `pio` and `sm` should match those passed to and returned by `nec_tx_init()`.
Note that the data bits are sent LSB first.
The function returns immediately unless the SM FIFO is full; however most receivers need you to pause at least 100ms before sending another frame.

### void nec_tx_raw(PIO pio, int sm, uint32_t data)
Send a raw 'NEC' IR frame with 32 bits of data.
_Note: to avoid confusing non-extended mode devices in the vicinity you should not send frames where the third and fourth bytes are each other's inverse._
The `pio` and `sm` should match those passed to and returned by `nec_tx_init()`.
Note that the data bits are sent LSB first.
The function returns immediately unless the SM FIFO is full; however most receivers need you to pause at least 100ms before sending another frame.

## Using the library in your own project
In general if you are using `CMake`:

1. Create a top level project directory with one sub-directory for your source files and another containing a copy of `nec_transmit`. Include a copy of
 `pico_sdk_import.cmake` and a `CMakelists.txt` based on the one in the repository.

1. Include `nec_transmit.h` in your source file(s).

1. Add `nec_transmit` to the list of target_link_libraries in the `CMakeLists.txt` of your source directory.
