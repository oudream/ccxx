###environment

set(fslSources
        crc/main.cpp
#        crc/test_check_sum1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(test_check
        ${fslSourcePaths})

set_target_properties(test_check PROPERTIES FOLDER ccxx/test)
