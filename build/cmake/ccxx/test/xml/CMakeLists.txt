
###has ccxx
set(fslSourceCcxxs
        xml/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtest_xml
        ${fslSourceCcxxPaths})

add_dependencies(cxtest_xml
        libccxx
        )

target_link_libraries(cxtest_xml
        libccxx
        )

set_target_properties(cxtest_xml PROPERTIES FOLDER ccxx/test)



