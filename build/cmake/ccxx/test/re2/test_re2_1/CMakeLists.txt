
###has ccxx
set(fslSourceCcxxs
        test_re2_1.cpp
        )

mc_merge_file_path(${gs_re2_cxtest_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtest_re2_1
        ${fslSourceCcxxPaths})

add_dependencies(cxtest_re2_1
        ccxx
        uv
        )

target_link_libraries(cxtest_re2_1
        ccxx
        uv
        )

set_target_properties(cxtest_re2_1 PROPERTIES FOLDER ccxx/test/re2)
