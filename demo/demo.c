/**
 * Copyright (c) 2021 mjcross
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "nec_transmit.h"                       // library header


int main() {
    PIO pio = pio0;                             // PIO to use
    uint gpio = 0;                              // GPIO for IR signal

    int sm = nec_tx_init(pio, gpio);            // initialise library

    if (sm == -1) {
        return -1;                              // error initialising library
    }

    /*
    generate "NEC" IR codes using 38kHz carrier
    (https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol)

    all the following examples generate the same bit sequence 'on the wire':
    0000 0000 1111 1111 1011 0101 0100 1010

    the functions return immediately unless the state machine FIFO fills up
    but most receivers need a little recovery time between frames

    note that in extended and raw modes you should avoid sending codes
    where the low and high address bytes are the inverse of each other
    otherwise you may interfere with other devices nearby
    */

    while (true) {

        // normal mode (8 bit address and data, lsb first)
        //      8 bit address,
        //      8 bit inverted address,
        //      8 bit data,
        //      8 bit inverted data
        nec_tx(pio, sm, 0x00, 0xad);
        sleep_ms(150);

        // extended mode (16 bit address and data, lsb first)
        //      16 bit address,
        //      16 bit data
        nec_tx_extended(pio, sm, 0xff00, 0x52ad);
        sleep_ms(150);

        // raw frame (32 bit data, lsb first)
        nec_tx_raw(pio, sm, 0x52adff00);
        sleep_ms(150);

    }
}
