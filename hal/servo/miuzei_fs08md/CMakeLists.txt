set(COMPONENT "servo_miuzei_fs08md")

file(GLOB SRCS "*.h" "*.cpp" )

add_library(${COMPONENT} STATIC ${SRCS})
target_link_libraries(${COMPONENT} PUBLIC pico_stdlib hardware_pwm servo ${PULSE_WIDTH_MODULATOR})
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PICO_CORE_INC_PATH})

add_subdirectory(test)