###environment

set(fslSources
        template/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(test_template
        ${fslSourcePaths})

set_target_properties(test_template PROPERTIES FOLDER ccxx/test)
