include_directories(${EXT_INSTALL_DIR}/include)
include_directories(${EXT_INSTALL_DIR}/lib)
include_directories(${EXT_INSTALL_DIR}/bin)

# cmake to build mpir, https://github.com/wbhart/mpir
include(mpir.cmake)

# Create a library called "Likelihood" which includes all .cpp files.
# The extension is already found
#file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )#"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
add_library(External ${SOURCES})

# Make sure the compiler can find include files for our Likelihood library
# when other libraries or executables link to Likelihood
target_include_directories (External PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_subdirectory(mpirbased)
#file(GLOB FAST_FACTORIAL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/mpirbased/*.cpp")
#add_library(FastFactorial ${FAST_FACTORIAL_SRC})
#add_dependencies(FastFactorial Mpir)
##target_include_directories(FastFactorial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
#target_link_libraries(FastFactorial Mpir)

