set(COMPONENT "sleeper")

file(GLOB SRCS "*.h")

add_library(${COMPONENT} INTERFACE ${SRCS})
target_include_directories(${COMPONENT} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(pico_sleeper)