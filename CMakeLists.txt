cmake_minimum_required(VERSION 3.12)

# pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# define the project and code standards
project(ir_transmit C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise the SDK
pico_sdk_init()

add_subdirectory(nec_transmit)
add_subdirectory(demo)
