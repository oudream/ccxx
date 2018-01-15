###environment

set(fslSources
        custom/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(cxtest_custom
        ${fslSourcePaths}
        )

add_dependencies(cxtest_custom
        libccxx
        )

target_link_libraries(cxtest_custom
        libccxx
        )

set_target_properties(cxtest_custom PROPERTIES FOLDER ccxx/test)
