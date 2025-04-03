file(GLOB SRCS "*.h" "*.cpp")

#set(PICO_SDK_HARWARE_PWM_INC ${PICO_SDK_PATH}/src/rp2_common/hardware_pwm/include)

#target_include_directories(${SRC_LIB} PUBLIC ${CMAKE_SOURCE_DIR}/inc ${PICO_CORE_INC_PATH} ${PICO_SDK_HARWARE_PWM_INC})

#target_link_libraries(${SRC_LIB} pico_stdlib ${PULSE_WIDTH_MODULATOR})

# create an executable that includes the file with the main function and link it to the previously create library
add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} pico_stdlib servo)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# configure to use stdout over usb
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})