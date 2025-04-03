set(COMPONENT "lidar")

file(GLOB SRCS "*.h" "*.cpp")

add_library(${COMPONENT} STATIC ${SRCS})
target_link_libraries(${COMPONENT} pico_stdlib hardware_uart)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PICO_CORE_INC_PATH})

add_subdirectory(test)