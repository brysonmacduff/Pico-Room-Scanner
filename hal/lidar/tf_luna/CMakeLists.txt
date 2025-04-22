set(COMPONENT "lidar_tf_luna")

file(GLOB SRCS "*.h" "*.cpp")

add_library(${COMPONENT} STATIC ${SRCS})
target_link_libraries(${COMPONENT} PUBLIC pico_stdlib hardware_uart lidar)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)