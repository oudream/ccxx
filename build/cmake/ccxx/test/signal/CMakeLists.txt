###environment

set(fslSources
        signal/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(test_signal
        ${fslSourcePaths})

set_target_properties(test_signal PROPERTIES FOLDER ccxx/test)



set(fslSources
        signal/lost1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(test_signal_lost1
        ${fslSourcePaths})

set_target_properties(test_signal_lost1 PROPERTIES FOLDER ccxx/test)
