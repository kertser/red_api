cmake_minimum_required(VERSION 3.10)
project(APICalculator C)
set(CMAKE_C_STANDARD 11)
set(API_NAME "red_api")

# Enable position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define output directories to ensure DLLs are with executables
if(WIN32)
    # On Windows, put everything in the same directory
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
    # On Linux, use standard locations
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# Setting 'include' and 'tools' are at the same level as 'build'
include_directories(include)
include_directories(tools)

# Library sources
file(GLOB LIB_SOURCES "libs/*.c")
list(APPEND LIB_SOURCES "tools/utils.c")

# Create shared library
add_library(${API_NAME} SHARED ${LIB_SOURCES})

# Main executable sources
set(MAIN_SOURCES "src/main.c" "tools/utils.c")
add_executable(APICalculator ${MAIN_SOURCES})

# Platform specific settings
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-DAPI_EXPORTS)

    # Set Windows-specific library properties
    set_target_properties(${API_NAME} PROPERTIES
            PREFIX ""          # No 'lib' prefix on Windows
            IMPORT_PREFIX ""   # No 'lib' prefix for import library
            )

    # Ensure the executable finds the DLL
    set_target_properties(APICalculator PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            )
else()
    add_definitions(-D_GNU_SOURCE)
    # Link with dl and math libraries on Linux
    target_link_libraries(APICalculator dl m)
    target_link_libraries(${API_NAME} dl m)

    # Set Linux-specific library properties
    set_target_properties(${API_NAME} PROPERTIES
            VERSION 1.0
            SOVERSION 1
            PREFIX "lib"
            )
endif()

target_link_libraries(APICalculator ${API_NAME})

# Adjust the absolute path to supported_systems.txt
set(SUPPORTED_SYSTEMS_PATH "${CMAKE_SOURCE_DIR}/src/supported_systems.txt")
add_definitions(-DSUPPORTED_SYSTEMS_PATH="${SUPPORTED_SYSTEMS_PATH}")

# Installation paths
if(WIN32)
    install(TARGETS APICalculator ${API_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION bin
            ARCHIVE DESTINATION lib
            )
else()
    install(TARGETS APICalculator ${API_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
endif()