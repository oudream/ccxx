###environment

set(fslSources
        file/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(cxtest_file
        ${fslSourcePaths}
        )

add_dependencies(cxtest_file
        ccxx
        )

target_link_libraries(cxtest_file
        ccxx
        )

set_target_properties(cxtest_file PROPERTIES FOLDER ccxx/test)
