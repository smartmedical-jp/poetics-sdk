project(poetics_test)

add_executable(poetics_test test.cpp)

# target_sources(tests PRIVATE test.cpp)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(poetics_test PRIVATE fmt::fmt-header-only)
set_property(TARGET poetics_test PROPERTY CXX_STANDARD 23)
add_test(poetics_test poetics_test)
