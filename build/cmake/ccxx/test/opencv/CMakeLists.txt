
#############################################

set(gs_opencv_test_path ${gs_ccxx_test_path}/opencv)

macro(mc_create_test1 sCppName)
    set(gsl_test_${sCppName}_sources
            ${sCppName}.cpp
            )

    mc_merge_file_path(${gs_opencv_test_path} "${gsl_test_${sCppName}_sources}"
            gsl_test_${sCppName}_source_filepaths)

    add_executable(cxtest_opencv_${sCppName}
            ${gsl_test_${sCppName}_head_filepaths} ${gsl_test_${sCppName}_source_filepaths}
            )

    add_dependencies(cxtest_opencv_${sCppName}
            ccxx
            )

    target_link_libraries(cxtest_opencv_${sCppName}
            ccxx
            )

    mc_target_link_opencv(cxtest_opencv_${sCppName})

    set_target_properties(cxtest_opencv_${sCppName} PROPERTIES FOLDER ccxx/test)
endmacro()

#set(v_name1 "ou")
#set(v_name2 "dream")
#set(${v_name1}${v_name2} "ouyangjinhong")
#message("oudream is " ${oudream})
#message("oudream is " ${${v_name1}${v_name2}})

#############################################

mc_create_test1(example1)
mc_create_test1(capture1)

