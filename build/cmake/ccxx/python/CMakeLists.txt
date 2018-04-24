
find_package (PythonInterp REQUIRED)
find_package (PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

mc_ccxx_simple_include()

### cxpython
set(gsl_cxpy_files
        cxpython/cxpy_module.cpp
        cxpython/cxpy_noddy2.h
        cxpython/cxpy_noddy2.cpp
        )
mc_merge_file_path(${gs_ccxx_path} "${gsl_cxpy_files}" gsl_cxpy_files_filepaths)
source_group("cxpy" FILES ${gsl_cxpy_files_filepaths})

add_library(cxpython SHARED
        ${gsl_cxpy_files_filepaths}
#        ${gsl_ccxx_simple_head_filepaths} ${gsl_ccxx_simple_source_filepaths}
        )

add_dependencies(cxpython
        ccxx
        )

target_link_libraries(cxpython
        ccxx
        )
if (WIN32)
    if (MSVC)
        target_link_libraries(cxpython
                ${PYTHON_LIBRARIES}
                )
    else()
#        target_link_libraries( IcsRt2Stack debug ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}/libgcl_sdk.dll)
#        target_link_libraries( IcsRt2Stack optimized ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}/libgcl_sdk.dll)
        target_link_libraries(cxpython
                C:/Python/Python36-32/libs/libpython36.a
                )
    endif()
else()
    target_link_libraries(cxpython
            ${PYTHON_LIBRARIES}
            )
endif ()

message("PYTHON_LIBRARIES: "  ${PYTHON_LIBRARIES})

set_target_properties(cxpython PROPERTIES FOLDER ccxx/lib)

#compile static
mc_build_with_cc_static()

