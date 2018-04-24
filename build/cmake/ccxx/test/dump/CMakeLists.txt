

### test dump1
set(fslDump1
        dump/test_dump1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslDump1}" fslDump1Path)

add_executable(cxtest_dump1
        ${fslDump1Path}
        )

add_dependencies(cxtest_dump1
        ccxx
        )

target_link_libraries(cxtest_dump1
        ccxx
        )

set_target_properties(cxtest_dump1 PROPERTIES FOLDER ccxx/test)

