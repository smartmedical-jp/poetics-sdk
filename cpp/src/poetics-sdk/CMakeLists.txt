project(poetics-sdk)

add_library(poetics-sdk SHARED poetics.cpp)

find_package(Poco REQUIRED COMPONENTS Foundation Net)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(poetics-sdk PRIVATE fmt::fmt-header-only)

target_link_libraries(poetics-sdk PRIVATE Poco::Foundation Poco::Net)

set_property(TARGET poetics-sdk PROPERTY CXX_STANDARD 20)

set(ARTIFACT_SRC_DIR "${CMAKE_BINARY_DIR}/poetics-sdk/${CMAKE_BUILD_TYPE}")
set(ARTIFACT_DST_DIR "${CMAKE_HOME_DIRECTORY}/lib")

add_custom_command(TARGET poetics-sdk POST_BUILD
    COMMAND powershell -File "${CMAKE_HOME_DIRECTORY}/copy_lib.ps1" -src_dir "${ARTIFACT_SRC_DIR}$<CONFIG>" -dst_root "${ARTIFACT_DST_DIR}" -build_type "$<CONFIG>"
    COMMENT "Executing PowerShell script..."
)
