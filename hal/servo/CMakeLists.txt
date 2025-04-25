set(COMPONENT "servo")

file(GLOB SRCS "*.h")

add_library(${COMPONENT} INTERFACE ${SRCS})
target_include_directories(${COMPONENT} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(miuzei_fs08md)