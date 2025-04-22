set(TEST_NAME ${COMPONENT}_test)

file(GLOB TEST_FILES "*.cpp")

add_executable(${TEST_NAME} ${TEST_FILES})
target_link_libraries(${TEST_NAME} PUBLIC ${COMPONENT} servo_miuzei_fs08md lidar_tf_luna)
target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# configure to use stdout over usb
pico_enable_stdio_usb(${TEST_NAME} 1)
pico_enable_stdio_uart(${TEST_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${TEST_NAME})