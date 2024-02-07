cmake_minimum_required(VERSION 3.10)
project(APICalculator C)
set(CMAKE_C_STANDARD 11)
set(API_NAME "red_api")

# Setting 'include' and 'tools' are at the same level as 'build'
include_directories(../include)
include_directories(../tools)

# Manually list source files
set(MAIN_SOURCES "../src/main.c" "../tools/utils.c")
add_executable(APICalculator ${MAIN_SOURCES})

# Automatically list all .c files in the libs folder for the library
file(GLOB LIB_SOURCES "../libs/*.c")
list(APPEND LIB_SOURCES "../tools/utils.c")
add_library(${API_NAME} SHARED ${LIB_SOURCES})

set_target_properties(${API_NAME} PROPERTIES VERSION 1.0 SOVERSION 1)
target_link_libraries(APICalculator ${API_NAME})

# Optional: Installation paths
install(TARGETS APICalculator DESTINATION bin)
install(TARGETS ${API_NAME} DESTINATION lib)