###environment

#############################################

macro(mc_create_test1 sCppName)
    set(gsl_test_${sCppName}_sources
            pipe/main_${sCppName}.cpp
            )

    mc_merge_file_path(${gs_ccxx_test_path} "${gsl_test_${sCppName}_sources}" gsl_test_${sCppName}_source_filepaths)

    add_executable(cxtest_pipe_${sCppName}
            ${gsl_test_${sCppName}_head_filepaths} ${gsl_test_${sCppName}_source_filepaths}
            )

    add_dependencies(cxtest_pipe_${sCppName}
            ccxx
            )

    target_link_libraries(cxtest_pipe_${sCppName}
            ccxx
            )

    set_target_properties(cxtest_pipe_${sCppName} PROPERTIES FOLDER ccxx/test)
endmacro()

macro(mc_create_test2 sCppName)
    set(gsl_test_${sCppName}_sources
            pipe/main_${sCppName}.cpp
            )

    mc_merge_file_path(${gs_ccxx_test_path} "${gsl_test_${sCppName}_sources}" gsl_test_${sCppName}_source_filepaths)

    add_executable(cxtest_pipe_${sCppName}
            ${gsl_test_${sCppName}_head_filepaths} ${gsl_test_${sCppName}_source_filepaths}
            )

    mc_target_link_libraries(cxtest_pipe_${sCppName})

    set_target_properties(cxtest_pipe_${sCppName} PROPERTIES FOLDER ccxx/test)
endmacro()

#############################################


if (APPLE)
endif ()

mc_create_test1(pipe1)
mc_create_test1(fork1)
