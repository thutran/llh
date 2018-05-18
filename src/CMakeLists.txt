# Create a library called "Likelihood" which includes all .cpp files.
# The extension is already found
#file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(Likelihood ${SOURCES})

# Make sure the compiler can find include files for our Likelihood library
# when other libraries or executables link to Likelihood
target_include_directories (Likelihood PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# file(GLOB SOURCES "${SRC}/*.cpp" "${SRC}/*.h")
# add_executable(${PROJECT_NAME} ${SOURCES} )

#find_package(GSL REQUIRED)