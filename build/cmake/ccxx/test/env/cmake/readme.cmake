return()

###WIN32
if(UNIX)
    find_program(TK_WISH cygwish80 )
endif()

###OUTPUT
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#You can also specify the output directories on a per target basis:

set_target_properties( targets...
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
#In both cases you can append _[CONFIG] to the variable/property name to make the output directory apply to a
#specific configuration (the standard values for config are DEBUG, RELEASE, MINSIZEREL and  RELWITHDEBINFO).

ADD_EXECUTABLE( myexe source1.cpp source2.cpp resource.rc )

###DEBUG
target_link_libraries(MyEXE
        debug Foo1_d optimized Foo1
        debug Foo2_d optimized Foo2)
#minsizerel
#relwithdebinfo
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${gs_project_output_path_debug}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${gs_project_output_path_release}")

if (COIN3D_LIBRARY_DEBUG AND COIN3D_LIBRARY_RELEASE)
    set(COIN3D_LIBRARIES optimized ${COIN3D_LIBRARY_RELEASE}
            debug ${COIN3D_LIBRARY_DEBUG})
else ()
    if (COIN3D_LIBRARY_DEBUG)
        set (COIN3D_LIBRARIES ${COIN3D_LIBRARY_DEBUG})
    endif ()
    if (COIN3D_LIBRARY_RELEASE)
        set (COIN3D_LIBRARIES ${COIN3D_LIBRARY_RELEASE})
    endif ()
endif ()

if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
endif()

IF(NOT CMAKE_COMPILER_IS_GNUCC)
    MESSAGE(FATAL_ERROR "Coverage can only be built on GCC")
ELSEIF(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE(FATAL_ERROR "Coverage can only be built in Debug mode")
ELSEIF(NOT LCOV_EXECUTABLE)
    MESSAGE(FATAL_ERROR "lcov executable not found")
ELSEIF(NOT GENHTML_EXECUTABLE)
    MESSAGE(FATAL_ERROR "genhtml executable not found")
ENDIF(NOT CMAKE_COMPILER_IS_GNUCC)

if(${CMAKE_ARCH} == 32)
    include(${CMAKE_BINARY_DIR}/release_32/conanbuildinfo.cmake)
else()
    include(${CMAKE_BINARY_DIR}/release_64/conanbuildinfo.cmake)
endif()

###math
set(i 0)
foreach (var ${vars})
    math(EXPR i ${i}+1)
endforeach ()
message(${i})

###targets
function (add_library name ....)
    # do stuff to remember this target
    _add_library (name ....)
endfunction (add_library)


###config
set(MY_PRODUCT_NUMBER 3)
set(MY_PRODUCT_VERSION 5)
set(MY_BUILD_NUMBER 49)

configure_file(
        ${gs_project_test_path}/test_qt1/version.rc.in
        ${gs_project_test_path}/test_qt1/version.rc
        @ONLY)