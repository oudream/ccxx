
###has ccxx
set(fslSourceCcxxs
        daemon/main.cpp
        )

mc_merge_file_path(${gs_ccxx_tools_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtool_daemon1
        ${fslSourceCcxxPaths})

mc_target_link_libraries(cxtool_daemon1 PROPERTIES FOLDER ccxx/tools)

set_target_properties(cxtool_daemon1 PROPERTIES FOLDER ccxx/tools)




###has ccxx
set(fslSourceDaemon2
        daemon/daemon_global.h
        daemon/daemon_global.cpp
        daemon/main_daemon.cpp
        )

mc_merge_file_path(${gs_ccxx_tools_path} "${fslSourceDaemon2}" fslSourceDaemon2Paths)

add_executable(cxtool_daemon2
        ${fslSourceDaemon2Paths})

add_dependencies(cxtool_daemon2
        ccxx
        )

target_link_libraries(cxtool_daemon2
        ccxx
        )

if (WIN32)
    target_link_libraries(cxtool_daemon2 ntdll)
endif ()

set_target_properties(cxtool_daemon2 PROPERTIES FOLDER ccxx/tools)

