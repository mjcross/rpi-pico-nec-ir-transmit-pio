add_executable (nec_transmit_demo demo.c)

target_link_libraries(nec_transmit_demo LINK_PUBLIC
  pico_stdlib
  hardware_pio
  nec_transmit
  )

pico_add_extra_outputs(nec_transmit_demo)
