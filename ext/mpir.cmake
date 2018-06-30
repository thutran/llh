find_file(MPIR_FILE_PATH mpir.h HINTS ${EXT_INSTALL_DIR}/include)
find_file(MPIR_LIB_PATH libmpir.a HINTS ${EXT_INSTALL_DIR}/lib)

# create a imported target library
add_library(Mpir STATIC IMPORTED GLOBAL)

# check if autoconf version >= 2.65; else install
execute_process(COMMAND autoconf --version
        COMMAND sed "1s/[A-Za-z() ]//g;q" # replace all characters except version number with empty string in the first line
        COMMAND tr -d \\n # remove trailing newline which was missed in sed
        OUTPUT_VARIABLE AUTOCONF_VERSION)
message(STATUS "Found autoconf version ${AUTOCONF_VERSION}")

if (NOT (${AUTOCONF_VERSION} VERSION_GREATER 2.64))
    set(AUTOCONF_PREFIX autoconf-2.69)
    set(AUTOCONF_URL ${CMAKE_CURRENT_SOURCE_DIR}/autoconf-2.69.tar.gz)
    ExternalProject_Add(${AUTOCONF_PREFIX}
            PREFIX ${AUTOCONF_PREFIX}
            URL ${AUTOCONF_URL}
            CONFIGURE_COMMAND sh configure --prefix=${EXT_INSTALL_DIR}
            BUILD_IN_SOURCE 1
            BUILD_COMMAND make
            TEST_BEFORE_INSTALL 0
            TEST_COMMAND ""
            INSTALL_DIR ${EXT_INSTALL_DIR}
            INSTALL_COMMAND
            )
    add_dependencies(Mpir ${AUTOCONF_PREFIX})
    include_directories(BEFORE ${EXT_INSTALL_DIR}/bin)

    message(STATUS "Install ${AUTOCONF_PREFIX} to ${EXT_INSTALL_DIR}/bin")
endif ()

if (${MPIR_FILE_PATH} STREQUAL "MPIR_FILE_PATH-NOTFOUND" OR
        ${MPIR_LIB_PATH} STREQUAL "MPIR_LIB_PATH-NOTFOUND")
    # case not found mpir

#    add_library(Mpir STATIC IMPORTED GLOBAL)

    ####### build yasm #######
    find_file(YASM_FILE_PATH yasm HINTS ${EXT_INSTALL_DIR}/bin)
    if(${YASM_FILE_PATH} STREQUAL "YASM_FILE_PATH-NOTFOUND")
#        set(YASM_PREFIX yasm)
        set(YASM_PREFIX yasm-1.3.0)
        set(YASM_URL ${CMAKE_CURRENT_SOURCE_DIR}/yasm-1.3.0.tar.gz)
        ExternalProject_Add(${YASM_PREFIX}
                PREFIX ${YASM_PREFIX}
                URL ${YASM_URL}
#                CONFIGURE_COMMAND sh configure --prefix=${YASM_INSTALL_DIR}
                CONFIGURE_COMMAND sh configure --prefix=${EXT_INSTALL_DIR}
                BUILD_COMMAND make
                BUILD_IN_SOURCE 1
                TEST_BEFORE_INSTALL 0
                TEST_COMMAND "" #make check
                INSTALL_DIR ${EXT_INSTALL_DIR}
                INSTALL_COMMAND #make install
                STEP_TARGETS autoconf
                )
        #ExternalProject_Get_Property(${YASM_PREFIX} INSTALL_DIR)
        #message(STATUS "Install dir of ${YASM_PREFIX} ${INSTALL_DIR}")
        ExternalProject_Get_Property(${YASM_PREFIX} SOURCE_DIR)
        ExternalProject_Add_Step(${YASM_PREFIX} autoconf
                COMMAND autoconf
                COMMENT "autoconf for yasm"
                DEPENDEES download
                DEPENDERS configure
                WORKING_DIRECTORY ${SOURCE_DIR}
                )

        ## add dependency so that yasm can be built
        add_dependencies(Mpir ${YASM_PREFIX})

#        find_file(YASM_FILE_PATH yasm HINTS ${EXT_INSTALL_DIR}/bin)
        set(YASM_FILE_PATH ${EXT_INSTALL_DIR}/bin/yasm)
        message(STATUS "Install yasm executable to ${EXT_INSTALL_DIR}/bin")

#        file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/${YASM_PREFIX})

    endif (${YASM_FILE_PATH} STREQUAL "YASM_FILE_PATH-NOTFOUND")
#    message(STATUS "yasm ${YASM_FILE_PATH}")

    ####### build mpir #######
#    set(MPIR_PREFIX mpir)
    set(MPIR_PREFIX mpir-3.0.0)
    set(MPIR_URL ${CMAKE_CURRENT_SOURCE_DIR}/mpir-3.0.0.tar.bz2)
#    set(MPIR_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/${MPIR_PREFIX}_installed)
    ExternalProject_Add(${MPIR_PREFIX}
            PREFIX ${MPIR_PREFIX}
            URL ${MPIR_URL}
#            CONFIGURE_COMMAND sh configure --with-yasm=${SOURCE_DIR}/yasm --enable-gmpcompat --prefix=${MPIR_INSTALL_DIR}
            CONFIGURE_COMMAND sh configure --with-yasm=${YASM_FILE_PATH} --prefix=${EXT_INSTALL_DIR}
            BUILD_COMMAND make
            BUILD_IN_SOURCE 1
            TEST_BEFORE_INSTALL 0
            TEST_COMMAND "" #make check
            INSTALL_DIR ${EXT_INSTALL_DIR}
            INSTALL_COMMAND #make install
            STEP_TARGETS autoconf
            )
    ExternalProject_Get_Property(${MPIR_PREFIX} SOURCE_DIR)
    ExternalProject_Add_Step(${MPIR_PREFIX} autoconf
            COMMAND autoconf
            COMMENT "autoconf for mpir"
            DEPENDEES download
            DEPENDERS configure
            WORKING_DIRECTORY ${SOURCE_DIR}
            )


#    ## add dependency so that yasm can be built
    add_dependencies(Mpir ${MPIR_PREFIX})

#    find_file(MPIR_FILE_PATH mpir.h HINTS ${EXT_INSTALL_DIR}/include)
#    find_file(MPIR_LIB_PATH libmpir.a HINTS ${EXT_INSTALL_DIR}/lib)
#    STRING(REGEX REPLACE "mpir\\.h$" "" MPIR_FILE_DIR ${MPIR_FILE_PATH})
    set(MPIR_FILE_PATH ${EXT_INSTALL_DIR}/include/mpir.h)
    set(MPIR_LIB_PATH ${EXT_INSTALL_DIR}/lib/libmpir.a)
    set(MPIR_FILE_DIR ${EXT_INSTALL_DIR}/include/)

#    set_property(TARGET Mpir PROPERTY IMPORTED_LOCATION ${EXT_INSTALL_DIR}/lib/libmpir.a)
    set_property(TARGET Mpir PROPERTY IMPORTED_LOCATION ${MPIR_LIB_PATH})
    #set_property(TARGET Mpir PROPERTY INCLUDE_DIRECTORIES ${MPIR_INSTALL_DIR}/include)
    set_property(TARGET Mpir PROPERTY INCLUDE_DIRECTORIES ${MPIR_FILE_DIR})
#    include_directories(${MPIR_FILE_DIR})
    #link_directories(${SOURCE_DIR}/include)
    message(STATUS "Install mpir.h to ${MPIR_FILE_DIR}")

#    file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/${MPIR_PREFIX})

else(${MPIR_FILE_PATH} STREQUAL "MPIR_FILE_PATH-NOTFOUND" OR
        ${MPIR_LIB_PATH} STREQUAL "MPIR_LIB_PATH-NOTFOUND")
    # case found mpir.h
    message(STATUS "Found mpir.h at ${MPIR_FILE_PATH}")
    message(STATUS "Found libmpir.a at ${MPIR_LIB_PATH}")

    STRING(REGEX REPLACE "mpir\\.h$" "" MPIR_FILE_DIR ${MPIR_FILE_PATH})
#    add_library(Mpir STATIC IMPORTED GLOBAL)
    set_property(TARGET Mpir PROPERTY IMPORTED_LOCATION ${MPIR_LIB_PATH})
    set_property(TARGET Mpir PROPERTY INCLUDE_DIRECTORIES ${MPIR_FILE_DIR})

    #    add_library(Mpir ${MPIR_FILE_PATH})
    #    target_link_libraries(Mpir ${MPIR_LIB_PATH})
    #    STRING(REGEX REPLACE "mpir\\.h$" "" MPIR_FILE_DIR ${MPIR_FILE_PATH})
    #    set_property(TARGET Mpir PROPERTY INCLUDE_DIRECTORIES ${MPIR_FILE_DIR})

endif (${MPIR_FILE_PATH} STREQUAL "MPIR_FILE_PATH-NOTFOUND" OR
        ${MPIR_LIB_PATH} STREQUAL "MPIR_LIB_PATH-NOTFOUND")

#message(STATUS "mpir.h ${MPIR_FILE_PATH}")