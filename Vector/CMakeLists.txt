include_directories("./")

# Build tests
enable_testing()

set(TARGET_NAME vector)
set(TEST_BINARY ${TARGET_NAME}_test)
set(VALGRIND_LOG "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_valgrind.log")

add_executable(${TEST_BINARY}
    test/TestVector.cpp
)

target_link_libraries(${TEST_BINARY}
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TEST_BINARY})

add_custom_target(
    memcheck_${TARGET_NAME}
    COMMAND valgrind
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --log-file=${VALGRIND_LOG}
            $<TARGET_FILE:${TEST_BINARY}> && tail -20 ${VALGRIND_LOG}
    DEPENDS ${TEST_BINARY}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running Valgrind memcheck on ${TEST_BINARY}, log: ${VALGRIND_LOG}"
    VERBATIM
)