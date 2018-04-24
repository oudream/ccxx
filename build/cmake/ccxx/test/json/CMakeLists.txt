
###has ccxx
set(fslSourceCcxxs
        json/main_ccxx.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtest_json
        ${fslSourceCcxxPaths})

add_dependencies(cxtest_json
        ccxx
        )

target_link_libraries(cxtest_json
        ccxx
        )

set_target_properties(cxtest_json PROPERTIES FOLDER ccxx/test)



###no ccxx
set(fslSources
        json/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(test_json
        ${fslSourcePaths})


set_target_properties(test_json PROPERTIES FOLDER ccxx/test)
