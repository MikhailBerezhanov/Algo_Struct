cmake_minimum_required(VERSION 3.14)

project(algo_struct)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip 
)

FetchContent_MakeAvailable(googletest)

# Build submodules 
add_subdirectory(Sorts)
add_subdirectory(LinkedList)
add_subdirectory(RingBuffer)
add_subdirectory(CycleBuffer)
add_subdirectory(Vector)