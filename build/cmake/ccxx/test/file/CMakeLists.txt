###environment

#############################################

macro(mc_create_test1 sCppName)
    set(gsl_test_${sCppName}_sources
            file/main_${sCppName}.cpp
            )

    mc_merge_file_path(${gs_ccxx_test_path} "${gsl_test_${sCppName}_sources}" gsl_test_${sCppName}_source_filepaths)

    add_executable(cxtest_${sCppName}
            ${gsl_test_${sCppName}_head_filepaths} ${gsl_test_${sCppName}_source_filepaths}
            )

    add_dependencies(cxtest_${sCppName}
            ccxx
            )

    target_link_libraries(cxtest_${sCppName}
            ccxx
            )

    set_target_properties(cxtest_${sCppName} PROPERTIES FOLDER ccxx/test)
endmacro()

#set(v_name1 "ou")
#set(v_name2 "dream")
#set(${v_name1}${v_name2} "ouyangjinhong")
#message("oudream is " ${oudream})
#message("oudream is " ${${v_name1}${v_name2}})

#############################################

mc_create_test1(fs1)
mc_create_test1(scan1)
mc_create_test1(diskspace1_win32)
mc_create_test1(diskspace2_win32)
mc_create_test1(diskspace3_win32)
mc_create_test1(system1)
