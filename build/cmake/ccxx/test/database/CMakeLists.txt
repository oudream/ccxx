###ccxx_odbc

set(fslOdbcSources
        database/main_odbc1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslOdbcSources}" fslOdbcSourcePaths)

add_executable(cxtest_database_odbc
        ${fslOdbcSourcePaths})

add_dependencies(cxtest_database_odbc
        ccxx
        ccxx_database_odbc
        )

target_link_libraries(cxtest_database_odbc
        ccxx
        ccxx_database_odbc
        )

set_target_properties(cxtest_database_odbc PROPERTIES FOLDER ccxx/test)




###ccxx_sqlite

set(fslSqliteSources
        database/main_sqlite1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSqliteSources}" fslSqliteSourcePaths)

add_executable(cxtest_database_sqlite
        ${fslSqliteSourcePaths})

add_dependencies(cxtest_database_sqlite
        ccxx
        ccxx_database_sqlite
        )

target_link_libraries(cxtest_database_sqlite
        ccxx
        ccxx_database_sqlite
        )

set_target_properties(cxtest_database_sqlite PROPERTIES FOLDER ccxx/test)




###ccxx_cursor

set(fslCursorSources
        database/main_cursor1.cpp
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslCursorSources}" fslCursorSourcePaths)

add_executable(cxtest_database_cursor
        ${fslCursorSourcePaths})

add_dependencies(cxtest_database_cursor
        ccxx
        ccxx_database_odbc
        ccxx_database_sqlite
        )

target_link_libraries(cxtest_database_cursor
        ccxx
        ccxx_database_odbc
        ccxx_database_sqlite
        )

set_target_properties(cxtest_database_cursor PROPERTIES FOLDER ccxx/test)
