
set(SOURCES
        algorithm/global.h
        algorithm/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${SOURCES}" fslSOURCESPaths)


add_executable(test_algorithm
        ${fslSOURCESPaths}
        )

set_target_properties(test_algorithm PROPERTIES FOLDER ccxx/test)

mc_target_link_libraries(test_algorithm)
