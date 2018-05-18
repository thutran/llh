cmake_minimum_required(VERSION 3.5)
project(likelihood)

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 1)

# configure file to pass some CMake settings to source code
configure_file(
        "${PROJECT_SOURCE_DIR}/ConfigCmake.h.in"
        "${PROJECT_BINARY_DIR}/ConfigCmake.h"
)

# add binary dir to the search path so that ConfigCmake.h can be found
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_subdirectory(src)
add_subdirectory(ext)
add_subdirectory(demo)

# set(SRC "${PROJECT_SOURCE_DIR}/src")
# include_directories(${SRC})
# file(GLOB SOURCES "${SRC}/*.cpp" "${SRC}/*.h")
# add_executable(${PROJECT_NAME} ${SOURCES} test/sampleTest.cpp)
