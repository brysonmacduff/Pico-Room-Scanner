set(TEST_FIRMWARE_NAME ${COMPONENT}_test)

file(GLOB SRCS "*.h" "*.cpp")

add_executable(${TEST_FIRMWARE_NAME} ${SRCS})
target_link_libraries(${TEST_FIRMWARE_NAME} PUBLIC lidar pico_cyw43_arch_none)
target_include_directories(${TEST_FIRMWARE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# configure to use stdout over usb
pico_enable_stdio_usb(${TEST_FIRMWARE_NAME} 1)
pico_enable_stdio_uart(${TEST_FIRMWARE_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${TEST_FIRMWARE_NAME})