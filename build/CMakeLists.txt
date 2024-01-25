cmake_minimum_required(VERSION 3.10)
project(APICalculator C)
set(CMAKE_C_STANDARD 11)
set(API_NAME "red_api")

# Assuming 'include' and 'tools' are at the same level as 'build'
include_directories(../include)
include_directories(../tools)

# Manually list your source files
set(MAIN_SOURCES "../src/main.c" "../tools/utils.c")
add_executable(APICalculator ${MAIN_SOURCES})

# Manually list your library sources
set(LIB_SOURCES "../libs/red_api.c" "../libs/RZ_104_11.c" "../tools/utils.c")
add_library(${API_NAME} SHARED ${LIB_SOURCES})

set_target_properties(${API_NAME} PROPERTIES VERSION 1.0 SOVERSION 1)
target_link_libraries(APICalculator ${API_NAME})

# Optional: Installation paths
install(TARGETS APICalculator DESTINATION bin)
install(TARGETS ${API_NAME} DESTINATION lib)