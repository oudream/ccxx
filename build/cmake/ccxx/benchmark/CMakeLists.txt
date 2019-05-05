
set(gs_benchmark_path ${gs_project_path}/benchmark)

# NOTE: Some tests use `<cassert>` to perform the test. Therefore we must
# strip -DNDEBUG from the default CMake flags in DEBUG mode.
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if( NOT uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG" )
    add_definitions( -UNDEBUG )
    add_definitions(-DTEST_BENCHMARK_LIBRARY_HAS_NO_ASSERTIONS)
    # Also remove /D NDEBUG to avoid MSVC warnings about conflicting defines.
    foreach (flags_var_to_scrub
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_MINSIZEREL)
        string (REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " "
                "${flags_var_to_scrub}" "${${flags_var_to_scrub}}")
    endforeach()
endif()

#check_cxx_compiler_flag(-O3 BENCHMARK_HAS_O3_FLAG)
#set(BENCHMARK_O3_FLAG "")
#if (BENCHMARK_HAS_O3_FLAG)
#    set(BENCHMARK_O3_FLAG "-O3")
#endif()

# NOTE: These flags must be added after find_package(Threads REQUIRED) otherwise
# they will break the configuration check.
if (DEFINED BENCHMARK_CXX_LINKER_FLAGS)
    list(APPEND CMAKE_EXE_LINKER_FLAGS ${BENCHMARK_CXX_LINKER_FLAGS})
endif()


include(${gs_project_3rd_path}/googletest/googletest/cmake/internal_utils.cmake)
config_compiler_and_linker()  # Defined in internal_utils.cmake.

macro(mc_create_test1 sCxxName)
    aux_source_directory(${gs_benchmark_path}/${sCxxName} gsl_${sCxxName}_DIR_SRCS)

    add_executable(benchmark_${sCxxName} ${gsl_${sCxxName}_DIR_SRCS})

    add_dependencies(benchmark_${sCxxName} benchmark ccxx)

    target_link_libraries(benchmark_${sCxxName} benchmark ${CMAKE_THREAD_LIBS_INIT} ccxx)

    set_target_properties(benchmark_${sCxxName} PROPERTIES FOLDER ccxx/benchmark)
endmacro()


mc_create_test1(cxstring)
