# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(poetics_sdk_cpp)

message("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
message("CMAKE_HOME_DIRECTORY=${CMAKE_HOME_DIRECTORY}")
message("CMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}")

# Find dependencies provided by vcpkg (via vcpkg.cmake)
find_package(fmt CONFIG REQUIRED)

# main targets
add_subdirectory(src/poetics-sdk poetics-sdk)
add_subdirectory(src/example_streaming example_streaming)

enable_testing()

# tests target
add_subdirectory(test test-sdk)
