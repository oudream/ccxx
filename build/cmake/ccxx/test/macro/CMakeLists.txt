###environment

set(fslSources
        macro/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(test_macro
        ${fslSourcePaths})

set_target_properties(test_macro PROPERTIES FOLDER ccxx/test)
