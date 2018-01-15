
set(SOURCES
        windows/global.h
        windows/main.cpp
#        windows/create_process1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_env_path} "${SOURCES}" fslSOURCESPaths)


add_executable(test_env_windows
        ${fslSOURCESPaths}
        )

set_target_properties(test_env_windows PROPERTIES FOLDER ccxx/test)

target_link_libraries(test_env_windows wtsapi32 userenv advapi32)
