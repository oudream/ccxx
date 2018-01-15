
set(fslSources
        dll/test_modbus_lib.h
        dll/test_modbus_lib.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_definitions(-D"VXD_API_MODBUS_IMPL_EXPORT")

add_library(test_dll_modbus SHARED
        ${fslSourcePaths}
        )

add_dependencies(test_dll_modbus
        libccxx
        test_dll_vdi
        )

target_link_libraries(test_dll_modbus
        libccxx
        test_dll_vdi
        )

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

#add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo" )

set_target_properties(test_dll_modbus PROPERTIES FOLDER ccxx/test)
