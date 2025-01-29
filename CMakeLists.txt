cmake_minimum_required(VERSION 3.10)
project(APICalculator C)
set(CMAKE_C_STANDARD 11)
set(API_NAME "red_api")

# Enable position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Read the JSON file content and configure header
file(READ "${CMAKE_SOURCE_DIR}/src/supported_systems.json" SUPPORTED_SYSTEMS_JSON)

# Configure the header file
configure_file(
        "${CMAKE_SOURCE_DIR}/include/supported_systems_data.h.in"
        "${CMAKE_BINARY_DIR}/include/supported_systems_data.h"
        @ONLY
)

# Add json-c
include(FetchContent)
FetchContent_Declare(
        json-c
        GIT_REPOSITORY https://github.com/json-c/json-c.git
        GIT_TAG json-c-0.17-20230812
)
set(BUILD_SHARED_LIBS ON)
set(BUILD_TESTING OFF CACHE BOOL "")
FetchContent_MakeAvailable(json-c)

# Define output directories
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/tools
        ${CMAKE_BINARY_DIR}/include
        ${json-c_SOURCE_DIR}
        ${json-c_BINARY_DIR}
)

# Library sources
file(GLOB LIB_SOURCES
        "libs/*.c"
        "tools/utils.c"
        "src/system_config.c"
        )

# Create shared library
add_library(${API_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${API_NAME} PRIVATE json-c)

# Main executable
add_executable(APICalculator "src/main.c" "tools/utils.c")

# Platform specific settings
if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0601 -DAPI_EXPORTS)

    set_target_properties(${API_NAME} PROPERTIES
            PREFIX ""
            IMPORT_PREFIX ""
            )
else()
    add_definitions(-D_GNU_SOURCE)

    set_target_properties(${API_NAME} PROPERTIES
            VERSION 1.0
            SOVERSION 1
            PREFIX "lib"
            )

    # Link with dl and math libraries on Linux
    target_link_libraries(${API_NAME} PRIVATE dl m)
    target_link_libraries(APICalculator PRIVATE dl m)
endif()

# Link the executable with our library and json-c
target_link_libraries(APICalculator PRIVATE ${API_NAME} json-c)

# Installation
install(TARGETS APICalculator ${API_NAME} json-c
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

# Install configuration files
install(FILES
        "${CMAKE_SOURCE_DIR}/src/supported_systems.json"
        DESTINATION share/${PROJECT_NAME}
        )