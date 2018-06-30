#cmake_minimum_required(VERSION 3.5)
cmake_minimum_required(VERSION 2.8)
project(likelihood)

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 2)

# from Nguyen's CMakeLists
IF (CMAKE_VERSION VERSION_LESS 3.1)
    MESSAGE("It is not greater than 3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
    endif ()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
else ()
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED YES)
endif (CMAKE_VERSION VERSION_LESS 3.1)

# configure file to pass some CMake settings to source code
configure_file(
        "${PROJECT_SOURCE_DIR}/ConfigCmake.h.in"
        "${PROJECT_BINARY_DIR}/ConfigCmake.h"
)

# add binary dir to the search path so that ConfigCmake.h can be found
include_directories("${PROJECT_BINARY_DIR}")

# build third-party libs,
# as instructed at
# https://www.selectiveintellect.net/blog/2016/7/29/using-cmake-to-add-third-party-libraries-to-your-project-1
# https://mirkokiefer.com/cmake-by-example-f95eb47d45b1
include(ExternalProject)

# directory to install third-party libs
set(EXT_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext_installed)

# to be passed to the compiler in order to be used in source code
if (TEST_FAST_FACTORIAL)
    add_definitions(-DTEST_FF=${TEST_FAST_FACTORIAL})
endif()



add_subdirectory(ext)
add_subdirectory(src)
add_subdirectory(demo)

# set(SRC "${PROJECT_SOURCE_DIR}/src")
# include_directories(${SRC})
# file(GLOB SOURCES "${SRC}/*.cpp" "${SRC}/*.h")
# add_executable(${PROJECT_NAME} ${SOURCES} test/sampleTest.cpp)
