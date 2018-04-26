
###has ccxx
set(fslSourceCcxxs
        tcp-echo-server/main.c
        )

mc_merge_file_path(${gs_libuv_cxtest_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtest_libuv_tcp
        ${fslSourceCcxxPaths})

add_dependencies(cxtest_libuv_tcp
        ccxx
        uv
        )

target_link_libraries(cxtest_libuv_tcp
        ccxx
        uv
        )

set_target_properties(cxtest_libuv_tcp PROPERTIES FOLDER ccxx/test/libuv)
