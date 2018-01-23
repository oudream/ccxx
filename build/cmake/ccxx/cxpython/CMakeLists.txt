
find_package (PythonInterp REQUIRED)
find_package (PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

mc_ccxx_simple_include()

###cxpy
set(gsl_cxpy_files
        cxpython/cxpy_module.cpp
        cxpython/cxpy_noddy2.h
        cxpython/cxpy_noddy2.cpp
        )
mc_merge_file_path(${gs_ccxx_path} "${gsl_cxpy_files}" gsl_cxpy_files_filepaths)
source_group("cxpy" FILES ${gsl_cxpy_files_filepaths})

add_library(cxpy SHARED
        ${gsl_cxpy_files_filepaths}
        ${gsl_ccxx_simple_head_filepaths} ${gsl_ccxx_simple_source_filepaths}
        )

if (WIN32)
    target_link_libraries(cxpy wsock32 ws2_32 psapi ${PYTHON_LIBRARIES})
endif ()

if (UNIX)
    target_link_libraries(cxpy dl rt uuid ${PYTHON_LIBRARIES})
endif ()

set_target_properties(cxpy PROPERTIES FOLDER ccxx/lib)

#compile static
mc_build_with_cc_static()

