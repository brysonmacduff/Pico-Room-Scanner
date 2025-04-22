file(GLOB SRCS "*.h" "*.cpp")

set(FIRMWARE_NAME "room_scanner")

add_executable(${FIRMWARE_NAME} ${SRCS})

target_link_libraries(${FIRMWARE_NAME} pico_stdlib pico_cyw43_arch_none scanner servo_miuzei_fs08md lidar_tf_luna pico_sleeper)
target_include_directories(${FIRMWARE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# configure to use stdout over usb
pico_enable_stdio_usb(${FIRMWARE_NAME} 1)
pico_enable_stdio_uart(${FIRMWARE_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${FIRMWARE_NAME})