include_directories("./")

# Build tests
enable_testing()

add_executable(slice_test
    test/TestSlice.cpp
)

target_link_libraries(slice_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(slice_test)