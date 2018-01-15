

### test channel udp client
set(fslCxLog1
        log/test_cxlog1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslCxLog1}" fslCxLog1Path)

add_executable(cxtest_cxlog1
        ${fslCxLog1Path}
        )

add_dependencies(cxtest_cxlog1
        libccxx
        )

target_link_libraries(cxtest_cxlog1
        libccxx
        )

set_target_properties(cxtest_cxlog1 PROPERTIES FOLDER ccxx/test)



