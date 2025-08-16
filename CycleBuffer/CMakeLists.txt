include_directories("./")

# Build tests
enable_testing()

add_executable(cycle_buffer_test
    test/TestCycleBuffer.cpp
)

target_link_libraries(cycle_buffer_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cycle_buffer_test)