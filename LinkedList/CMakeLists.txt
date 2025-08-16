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

add_executable(doubly_linked_list_test
    test/TestDoublyLinkedList.cpp
)

target_link_libraries(doubly_linked_list_test
    GTest::gtest_main
)

gtest_discover_tests(doubly_linked_list_test)