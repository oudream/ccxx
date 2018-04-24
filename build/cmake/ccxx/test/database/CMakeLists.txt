###ccxx_odbc

set(fslSources
        database/main.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

add_executable(cxtest_database
        ${fslSourcePaths})

add_dependencies(cxtest_database
        ccxx
        ccxx_database_odbc
        ccxx_database_sqlite)

target_link_libraries(cxtest_database
        ccxx
        ccxx_database_odbc
        ccxx_database_sqlite)

set_target_properties(cxtest_database PROPERTIES FOLDER ccxx/test)

