set(TEST_NAME ${COMPONENT}_test)

file(GLOB TEST_FILES "*.cpp")

set(GTEST_LIBRARY "/usr/lib/x86_64-linux-gnu/libgtest.a")
set(GTEST_MAIN_LIBRARY "/usr/lib/x86_64-linux-gnu/libgtest_main.a")
set(GTEST_INC_DIR "/usr/include/gmock")
set(GMOCK_LIBRARY "/usr/lib/x86_64-linux-gnu/libgmock.a")
set(GMOCK_MAIN_LIBRARY "/usr/lib/x86_64-linux-gnu/libgmock_main.a")
set(GMOCK_INC_DIR "/usr/include/gtest")

include(CTest)
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(${TEST_NAME} ${TEST_FILES})
target_link_libraries(${TEST_NAME} PUBLIC ${COMPONENT} GTest::gtest_main GTest::gmock_main)
target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

gtest_discover_tests(${TEST_NAME})