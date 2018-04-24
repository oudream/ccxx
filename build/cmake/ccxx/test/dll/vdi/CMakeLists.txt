
set(fslSources
        dll/test_vdi_lib.h
        dll/test_vdi_lib.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_definitions(-D"GM_BUILD_GCL_VDI_LIB")

add_library(test_dll_vdi SHARED
        ${fslSourcePaths}
        )

add_dependencies(test_dll_vdi
        ccxx
        )

target_link_libraries(test_dll_vdi
        ccxx
        )

set_target_properties(test_dll_vdi PROPERTIES FOLDER ccxx/test)

