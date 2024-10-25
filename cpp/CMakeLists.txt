# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
if (WIN32)
    cmake_policy(SET CMP0091 NEW) # https://github.com/microsoft/vcpkg/issues/18531#issuecomment-864597848
endif (WIN32)
project(poetics_sdk_cpp)

message("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
message("CMAKE_HOME_DIRECTORY=${CMAKE_HOME_DIRECTORY}")
message("CMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}")

SET(POETICS_OUT_PATH ${CMAKE_HOME_DIRECTORY}/out)

# main target
add_subdirectory(src/poetics_sdk)
add_subdirectory(src/poetics_example)

enable_testing()
# tests target
add_subdirectory(src/poetics_test)
