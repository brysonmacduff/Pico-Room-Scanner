cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH

set(PICO_BOARD pico_w)
set(PICO_SDK_PATH "/home/bryson/projects/raspberry_pi_pico_w/pico-sdk")
set(PICO_CORE_LIB_PATH "/usr/lib/pico-core")
set(PICO_CORE_INC_PATH "/usr/include/pico-core")

# note: this must happen before project()
include(pico_sdk_import.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

project(room_scanner C CXX ASM)

find_library(PULSE_WIDTH_MODULATOR
  NAMES pulse_width_modulator
  PATHS ${PICO_CORE_LIB_PATH}
)

# include testing dependencies
#set(GTEST_ROOT /usr/src/googletest)
#include(CTest)
#find_package(GTest REQUIRED)
#find_package(spdlog REQUIRED)
#include(GoogleTest)

add_subdirectory(servo)
add_subdirectory(lidar)
add_subdirectory(src)
