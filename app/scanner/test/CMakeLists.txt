set(TEST_NAME ${COMPONENT}_test)

file(GLOB TEST_FILES "*.cpp")

add_executable(${TEST_NAME} ${TEST_FILES})

target_link_libraries(${TEST_NAME} PUBLIC ${COMPONENT} GTest::gtest_main spdlog::spdlog)
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})

target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})