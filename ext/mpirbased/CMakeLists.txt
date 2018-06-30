find_package(Threads) # needs in parallelswing

file(GLOB FAST_FACTORIAL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(FastFactorial ${FAST_FACTORIAL_SRC})

add_dependencies(FastFactorial Mpir)

target_link_libraries(FastFactorial PUBLIC Mpir)
target_link_libraries(FastFactorial PUBLIC ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(FastFactorial BEFORE PUBLIC ${EXT_INSTALL_DIR}/include)
target_include_directories(FastFactorial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#get_property(tmp TARGET Mpir PROPERTY INCLUDE_DIRECTORIES)
#message(${tmp})