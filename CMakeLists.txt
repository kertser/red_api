# Set the minimum required version of cmake for this project
cmake_minimum_required(VERSION 3.10)

# Define the project name and the programming language (C in this case)
project(APICalculator C)

# Set the C standard for the project
set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c red_api.c)

# Set the output directory for the DLL
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Link directories
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(red_api red_api.c)

# --- enable for dll generation ---

# Create a shared library (DLL) from the source files
#add_library(APICalculator SHARED ${SOURCE_FILES})

#if(WIN32)
#   target_compile_definitions(APICalculator PRIVATE -DMYLIBRARY_EXPORTS)
#endif()

# --- enable for dll generation ---

# Add the executable that is built from the source files
# The first argument is the name of the executable and the rest are source files
# -- disable for dll generation
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

# Specify the output directory for executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output)

# Similarly, if you need to link against libraries, you can specify them using target_link_libraries
target_link_libraries(${PROJECT_NAME} red_api)
