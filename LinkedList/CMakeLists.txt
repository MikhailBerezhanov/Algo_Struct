include_directories("./")

# Build tests
enable_testing()

add_executable(linked_list_test
    test/TestForwardList.cpp
)

target_link_libraries(linked_list_test
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(linked_list_test)