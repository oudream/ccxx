###environment

set(fslSources
        timer/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(cxtest_timer
        ${fslSourcePaths}
        )

add_dependencies(cxtest_timer
        ccxx
        )

target_link_libraries(cxtest_timer
        ccxx
        )

set_target_properties(cxtest_timer PROPERTIES FOLDER ccxx/test)
