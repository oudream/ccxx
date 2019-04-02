
###
### Sources, headers, directories and libs
###

# From http://www.cmake.org/pipermail/cmake/2010-March/035992.html:
# function to collect all the sources from sub-directories
# into a single list
function(add_sources)
    get_property(is_defined GLOBAL PROPERTY SRCS_LIST DEFINED)
    if(NOT is_defined)
        define_property(GLOBAL PROPERTY SRCS_LIST
                BRIEF_DOCS "List of source files"
                FULL_DOCS "List of all source files in the entire project")
    endif()
    # make absolute paths
    set(SRCS)
    foreach(s IN LISTS ARGN)
        if(NOT IS_ABSOLUTE "${s}")
            get_filename_component(s "${s}" ABSOLUTE)
        endif()
        list(APPEND SRCS "${s}")
    endforeach()
    # append to global list
    set_property(GLOBAL APPEND PROPERTY SRCS_LIST "${SRCS}")
endfunction(add_sources)

###
### yaml-cpp yamlcpp Library
###
file(GLOB sources "src/[a-zA-Z]*.cpp")
file(GLOB_RECURSE public_headers "include/yaml-cpp/[a-zA-Z]*.h")
file(GLOB private_headers "src/[a-zA-Z]*.h")

set(library_sources
        ${sources}
        ${public_headers}
        ${private_headers}
        )
add_sources(${library_sources})
add_library(yaml-cpp ${library_sources})


#include(ExternalProject)

###
### test
###
#ExternalProject_Add(
#        googletest_project
#        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../googletest"
#        INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/prefix"
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
#        -DBUILD_GMOCK=ON
#        -Dgtest_force_shared_crt=ON
#)

file(GLOB test_headers test/[a-z_]*.h)
file(GLOB test_sources test/[a-z_]*.cpp test/integration/[a-z_]*.cpp test/node/[a-z_]*.cpp)
file(GLOB test_new_api_sources test/new-api/[a-z]*.cpp)

list(APPEND test_sources ${test_new_api_sources})
add_sources(${test_sources} ${test_headers})

include_directories(${gs_yamlcpp_src_path})
include_directories(${gs_yamlcpp_test_path})

find_package(Threads)

add_executable(yaml-cpp-gtest
        ${test_sources}
        ${test_headers}
        )

add_dependencies(yaml-cpp-gtest gmock)

set_target_properties(yaml-cpp-gtest PROPERTIES LINKER_LANGUAGE CXX)
#set_target_properties(yaml-cpp-gtest PROPERTIES
#        COMPILE_FLAGS "${yaml_c_flags} ${yaml_cxx_flags} ${yaml_test_flags}"
#        )
target_link_libraries(yaml-cpp-gtest
        yaml-cpp
        gmock
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_test(yaml-test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/yaml-cpp-gtest)
