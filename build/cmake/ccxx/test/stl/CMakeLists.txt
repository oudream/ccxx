###environment

set(fslSources
        stl/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(test_stl
        ${fslSourcePaths})

set_target_properties(test_stl PROPERTIES FOLDER ccxx/test)
