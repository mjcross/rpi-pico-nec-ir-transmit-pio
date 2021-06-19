#include "pico/stdlib.h"
#include "hardware/pio.h"

// public API
//
int nec_tx_init(PIO, uint);
void nec_tx(PIO, int, uint8_t, uint8_t);
void nec_tx_extended(PIO, int, uint16_t, uint16_t);
void nec_tx_raw(PIO, int, uint32_t);
