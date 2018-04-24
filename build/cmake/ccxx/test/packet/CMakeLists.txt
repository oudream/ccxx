###environment

set(fslSources
        packet/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(cxtest_packet
        ${fslSourcePaths}
        )

add_dependencies(cxtest_packet
        ccxx
        )

target_link_libraries(cxtest_packet
        ccxx
        )

set_target_properties(cxtest_packet PROPERTIES FOLDER ccxx/test)
