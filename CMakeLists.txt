cmake_minimum_required(VERSION 3.22)
project(MagewellCaptureSDK)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    pthread
    udev
    asound
    v4l2
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

# Build a test driver from the test sources.
enable_testing()
create_test_sourcelist(magewell_capture_tests
    test_driver.cpp
    test/get_version.cpp
    test/init_instance.cpp
    test/exit_instance.cpp
    test/refresh_device.cpp
    test/channel_iterator.cpp
    test/open_channel.cpp
)
add_executable(TestDriver ${magewell_capture_tests})
target_link_libraries(TestDriver PRIVATE MagewellCapture)
add_test(NAME get_version COMMAND TestDriver test/get_version)
add_test(NAME init_instance COMMAND TestDriver test/init_instance)
add_test(NAME exit_instance COMMAND TestDriver test/exit_instance)
add_test(NAME refresh_device COMMAND TestDriver test/refresh_device)
add_test(NAME channel_iterator COMMAND TestDriver test/channel_iterator)
add_test(NAME open_channel COMMAND TestDriver test/open_channel)

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

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_USE_MATHJAX YES)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    set(DOXYGEN_IMAGE_PATH ${CMAKE_SOURCE_DIR}/images)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_EXCLUDE
        ${CMAKE_BINARY_DIR}
        # Ignore the underlying Magewell SDK header files. Magewell document
        # them separately.
        ${CMAKE_SOURCE_DIR}/Include
    )
    doxygen_add_docs(Doxygen ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        ALL
    )
endif()
