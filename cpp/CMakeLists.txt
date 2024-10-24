# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(poetics_sdk_cpp)

message("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
message("CMAKE_HOME_DIRECTORY=${CMAKE_HOME_DIRECTORY}")
message("CMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}")

# main target
add_subdirectory(src/poetics_standalone)

enable_testing()
# tests target
add_subdirectory(src/poetics_test)
