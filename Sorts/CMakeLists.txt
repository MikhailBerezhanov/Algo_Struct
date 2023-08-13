include_directories("./")

# Build tests
enable_testing()

add_executable(sorts_test
    test/TestSorts.cpp
)

target_link_libraries(sorts_test
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(sorts_test)