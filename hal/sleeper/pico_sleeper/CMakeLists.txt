set(COMPONENT "pico_sleeper")

file(GLOB SRCS "*.h" "*.cpp")

add_library(${COMPONENT} STATIC ${SRCS})
target_link_libraries(${COMPONENT} PUBLIC sleeper pico_stdlib)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)