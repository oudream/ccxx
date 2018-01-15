
set(SOURCES
        algorithm/global.h
        algorithm/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${SOURCES}" fslSOURCESPaths)


add_executable(test_algorithm
        ${fslSOURCESPaths}
        )

set_target_properties(test_algorithm PROPERTIES FOLDER ccxx/test)

if (WIN32)
    target_link_libraries(test_algorithm wsock32 ws2_32 psapi)
endif ()

if (UNIX)
    target_link_libraries(test_algorithm dl rt uuid)
endif ()