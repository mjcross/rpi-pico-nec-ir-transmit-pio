/**
 * Copyright (c) 2021 mjcross
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


// SDK types and declarations
//
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"    // for clock_get_hz()


// public API declarations
//
#include "nec_transmit.h"


// PIO state machine programs:
//
#include "nec_carrier_burst.pio.h"
#include "nec_carrier_control.pio.h"



// public API definitions
//
int nec_tx_init(PIO pio, uint pin_num) {
    // initialise PIO to transmit NEC IR frames on specified GPIO pin
    // returns state machine number on success, otherwise -1 if an error occurred

    // load and run carrier burst program
    //
    uint carrier_burst_offset = pio_add_program(pio, &nec_carrier_burst_program);
    int carrier_burst_sm = pio_claim_unused_sm(pio, true);

    if (carrier_burst_sm == -1) {
        return -1;
    }

    nec_carrier_burst_program_init(pio,
                                   carrier_burst_sm,
                                   carrier_burst_offset,
                                   pin_num,
                                   38.222e3);                   // 38.222 kHz carrier

    // load and run carrier control program
    //
    uint carrier_control_offset = pio_add_program(pio, &nec_carrier_control_program);
    int carrier_control_sm = pio_claim_unused_sm(pio, true);

    if (carrier_control_sm == -1) {
        return -1;
    }

    nec_carrier_control_program_init(pio,
                                     carrier_control_sm,
                                     carrier_control_offset,
                                     2 * (1 / 562.5e-6),        // 2 state machine ticks per 562.5us carrier burst
                                     32);                       // send 32 data bits per frame (MSB first)

    return carrier_control_sm;
}


void nec_tx(PIO pio, int sm, uint8_t address, uint8_t data) {
    // 8 bits address, 8 bits inverse address, 8 bits data, 8 bits inverse data
    // data is sent LSB first
    // the inverse bits provide a crude checksum and keep the overall frame length constant at 68ms
    pio_sm_put_blocking(pio, sm, address | (address ^ 0xff) << 8 | data << 16 | (data ^ 0xff) << 24);
}


void nec_tx_extended(PIO pio, int sm, uint16_t address, uint16_t data) {
    // 16 bits address, 16 bits data
    // data is sent LSB first
    // NOTE: to avoid confusing non-extended mode devices in the vicinity you should not send
    // frames where the upper eight bits of the address are the inverse of its lower eight.
    pio_sm_put_blocking(pio, sm, address | data << 16);
}


void nec_tx_raw(PIO pio, int sm, uint32_t data) {
    // raw 32 bit frame
    // data is sent LSB first
    pio_sm_put_blocking(pio, sm, data);
}
