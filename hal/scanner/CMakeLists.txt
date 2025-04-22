set(COMPONENT "scanner")

# This pico arm64 library will draw its source code from the app layer above the hal
file(GLOB SRCS "*.h" "*.cpp")

add_library(${COMPONENT} STATIC ${SRCS})
target_link_libraries(${COMPONENT} PUBLIC servo lidar sleeper)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)