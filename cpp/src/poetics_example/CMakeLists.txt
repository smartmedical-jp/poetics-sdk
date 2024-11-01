project(poetics_example)

set(SOURCES
    main.cpp)
include_directories(${CMAKE_HOME_DIRECTORY}/src/poetics_sdk/include)
link_directories(${POETICS_OUT_PATH})

set(EXECUTABLE_OUTPUT_PATH ${POETICS_OUT_PATH})

add_executable(poetics_example ${SOURCES})

find_package(Poco REQUIRED COMPONENTS Foundation Net NetSSL)
target_link_libraries(poetics_example PRIVATE Poco::Foundation Poco::Net Poco::NetSSL)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(poetics_example PRIVATE fmt::fmt-header-only)

target_link_libraries(poetics_example PRIVATE poetics_sdk)

set_property(TARGET poetics_example PROPERTY CXX_STANDARD 20)
