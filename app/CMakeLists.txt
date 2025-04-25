cmake_minimum_required(VERSION 3.13)
project(app LANGUAGES C CXX ASM)

include(CTest)
find_package(GTest REQUIRED)
include(GoogleTest)

find_package(spdlog REQUIRED)

add_subdirectory(scanner)