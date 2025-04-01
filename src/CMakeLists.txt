file(GLOB SRCS "*.h" "*.cpp")

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/inc)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_uart ${PULSE_WIDTH_MODULATOR})

# configure to use stdout over usb
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})