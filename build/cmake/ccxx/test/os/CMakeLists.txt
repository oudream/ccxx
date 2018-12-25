###environment

#############################################

macro(mc_create_test1 sCppName)
    set(gsl_test_${sCppName}_sources
            os/main_${sCppName}.cpp
            )

    mc_merge_file_path(${gs_ccxx_test_path} "${gsl_test_${sCppName}_sources}" gsl_test_${sCppName}_source_filepaths)

    add_executable(cxtest_os_${sCppName}
            ${gsl_test_${sCppName}_head_filepaths} ${gsl_test_${sCppName}_source_filepaths}
            )

    add_dependencies(cxtest_os_${sCppName}
            ccxx
            )

    target_link_libraries(cxtest_os_${sCppName}
            ccxx
            )

    set_target_properties(cxtest_os_${sCppName} PROPERTIES FOLDER ccxx/test)
endmacro()

macro(mc_create_test2 sCppName)
    set(gsl_test_${sCppName}_sources
            os/main_${sCppName}.cpp
            )

    mc_merge_file_path(${gs_ccxx_test_path} "${gsl_test_${sCppName}_sources}" gsl_test_${sCppName}_source_filepaths)

    add_executable(cxtest_os_${sCppName}
            ${gsl_test_${sCppName}_head_filepaths} ${gsl_test_${sCppName}_source_filepaths}
            )

    mc_target_link_libraries(cxtest_os_${sCppName})

    set_target_properties(cxtest_os_${sCppName} PROPERTIES FOLDER ccxx/test)
endmacro()

#############################################


if (APPLE)
    mc_create_test2(sysinfo1_darwin)
endif ()
