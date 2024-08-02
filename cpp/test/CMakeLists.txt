project(test-sdk)

add_executable(test-sdk test.cpp)

# target_sources(tests PRIVATE test.cpp)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(test-sdk PRIVATE fmt::fmt-header-only)
set_property(TARGET test-sdk PROPERTY CXX_STANDARD 23)
add_test(test-sdk test-sdk)