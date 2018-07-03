# Add executable called "llhDemo" that is built from the source files
# "demo.cxx". The extensions are automatically found.
add_executable (llhDemo demo.cpp)

# GSL libraries
find_package(GSL REQUIRED)
#if (GSL_FOUND)
#    include_directories(${GSL_INCLUDE_DIRS})
#endif ()

# Link the executable to the Likelihood library. Since the Likelihood library has
# public include directories we will use those link directories when building llhDemo
#target_link_libraries(llhDemo PRIVATE Likelihood External GSL::gsl GSL::gslcblas)
if(${FAST_FACTORIAL_ON})
    add_definitions(-DFF_ON=${FAST_FACTORIAL_ON})
    target_link_libraries(llhDemo PRIVATE Likelihood External FastFactorial GSL::gsl GSL::gslcblas)
else()
    target_link_libraries(llhDemo PRIVATE Likelihood External GSL::gsl GSL::gslcblas)
endif()
