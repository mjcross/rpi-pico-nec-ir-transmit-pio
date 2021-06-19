
add_library(nec_transmit nec_transmit.c)

# invoke pio_asm to assemble state machine code
pico_generate_pio_header(nec_transmit ${CMAKE_CURRENT_LIST_DIR}/nec_carrier_burst.pio)
pico_generate_pio_header(nec_transmit ${CMAKE_CURRENT_LIST_DIR}/nec_carrier_control.pio)

target_link_libraries(nec_transmit PRIVATE
        pico_stdlib
        hardware_pio
        )

target_include_directories (nec_transmit PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)
