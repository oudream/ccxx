
set(fslSources
        dll/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(test_dll_host
        ${fslSourcePaths}
        )

add_dependencies(test_dll_host
        libccxx
        test_dll_vdi
        )

target_link_libraries(test_dll_host
        libccxx
        test_dll_vdi
        )

set_target_properties(test_dll_host PROPERTIES FOLDER ccxx/test)

