###environment

set(fslSources
        string/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(cxtest_string
        ${fslSourcePaths}
        )

add_dependencies(cxtest_string
        ccxx
        )

target_link_libraries(cxtest_string
        ccxx
        )

set_target_properties(cxtest_string PROPERTIES FOLDER ccxx/test)
