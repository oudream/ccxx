
### cxsample_callpython
set(gsl_cxsample_callpython_files
        module1.h
        module1.cpp
        main.cpp
        )

mc_merge_file_path(${gs_ccxx_sample_path}/callpython "${gsl_cxsample_callpython_files}" gsl_cxsample_callpython_files_filepaths)

add_executable(cxsample_callpython
        ${gsl_cxsample_callpython_files_filepaths}
        )

add_custom_command(
        TARGET cxsample_callpython
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${gs_ccxx_sample_path}/callpython/multiply.py
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/multiply.py
        )

target_link_libraries(cxsample_callpython
        ${PYTHON_LIBRARIES}
        )

mc_target_link_libraries(cxsample_callpython)

#compile static
mc_build_with_cc_static()
