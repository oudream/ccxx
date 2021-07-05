###environment

set(fslSources
        winreg/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(cxtest_winreg
        ${fslSourcePaths}
        )

add_dependencies(cxtest_winreg
        ccxx
        )

target_link_libraries(cxtest_winreg
        ccxx
        )

set_target_properties(cxtest_winreg PROPERTIES FOLDER ccxx/test)
