project(example_streaming)

# find_library(poetics-sdk poetics-sdk HINTS "D:/REPO/poetics-sdk-cpp/builds/ninja-multi-vcpkg/poetics-sdk/Debug")
add_executable(example_streaming main.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net)
target_link_libraries(example_streaming PRIVATE Poco::Foundation Poco::Net)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(example_streaming PRIVATE fmt::fmt-header-only)

set_property(TARGET example_streaming PROPERTY CXX_STANDARD 20)
