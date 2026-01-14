cmake_minimum_required(VERSION 3.22)
project(MagewellCaptureSDK)

add_library(MagewellCapture SHARED
    ${CMAKE_SOURCE_DIR}/inc/MagewellCapture.h
    ${CMAKE_SOURCE_DIR}/src/MagewellCapture.cpp
)
target_include_directories(MagewellCapture PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(MagewellCapture PRIVATE
    MWCapture
)

# Import the MWCapture static library. Its location depends on
# architecture. Assume the library files are located in "Lib/x64" and
# "Lib/arm64" respectively. Presume only x86_64 and aarch64 are
# supported, and that the current system architecture matches one of
# them; ignore cross-compilation.
set(ARCH_DIR "")
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(ARCH_DIR "x64")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(ARCH_DIR "arm64")
else()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()
add_library(MWCapture STATIC IMPORTED)
set_target_properties(MWCapture PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Lib/${ARCH_DIR}/libMWCapture.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Include/LibMWCapture"
)

create_test_sourcelist(magewell_capture_tests
    test_driver.cpp
    test/get_version.cpp
)
add_executable(test_driver ${magewell_capture_tests})
target_link_libraries(test_driver PRIVATE MagewellCapture)
add_test(NAME get_version COMMAND test_driver test/get_version)

# Installation rules.
# Install header files.
install(DIRECTORY ${CMAKE_SOURCE_DIR}/inc/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
install(TARGETS MagewellCapture
    EXPORT MagewellCaptureConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
