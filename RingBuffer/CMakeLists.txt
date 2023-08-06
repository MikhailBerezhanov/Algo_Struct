include_directories("./")

# Build tests
enable_testing()

add_executable(ring_buffer_test
    test/TestRingBuffer.cpp
)

target_link_libraries(ring_buffer_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ring_buffer_test)