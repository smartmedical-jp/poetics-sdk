project(poetics_test)

add_executable(poetics_test test.cpp)
set(EXECUTABLE_OUTPUT_PATH ${POETICS_OUT_PATH})

# target_sources(tests PRIVATE test.cpp)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(poetics_test PRIVATE fmt::fmt-header-only)
set_property(TARGET poetics_test PROPERTY CXX_STANDARD 23)
add_test(poetics_test poetics_test)
