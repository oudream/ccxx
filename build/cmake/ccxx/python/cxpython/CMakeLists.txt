
### cxpython
set(gsl_cxpython_files
        cxpy_module.cpp
        cxpy_noddy2.cpp
        cxpy_noddy2.h
        )


mc_merge_file_path(${gs_ccxx_source_path}/cxpython "${gsl_cxpython_files}" gsl_cxpython_files_filepaths)

add_executable(cxpython
        ${gsl_cxpython_files_filepaths}
        )

target_link_libraries(cxpython
        ${PYTHON_LIBRARIES}
        )

mc_target_link_libraries(cxpython)

#compile static
mc_build_with_cc_static()
