
###has ccxx
set(fslSourceCcxxs
        libuv/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtest_libuv
        ${fslSourceCcxxPaths})

add_dependencies(cxtest_libuv
        ccxx
        uv
        )

target_link_libraries(cxtest_libuv
        ccxx
        uv
        )

set_target_properties(cxtest_libuv PROPERTIES FOLDER ccxx/test)



