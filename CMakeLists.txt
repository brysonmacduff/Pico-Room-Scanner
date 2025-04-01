cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH

set(PICO_BOARD pico_w)
set(PICO_SDK_PATH "/home/bryson/projects/raspberry_pi_pico_w/pico-sdk")
set(PICO_CORE_PATH "/usr/lib/pico-core")

# note: this must happen before project()
include(pico_sdk_import.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

project(room_scanner C CXX ASM)

find_library(PULSE_WIDTH_MODULATOR
  NAMES pulse_width_modulator
  PATHS ${PICO_CORE_PATH}
)


add_subdirectory(inc)
add_subdirectory(src)
