
set(gs_cxsample_cmd_sources
        cmd/main.cpp
        )

mc_merge_file_path(${gs_ccxx_sample_path} "${gs_cxsample_cmd_sources}" gs_cxsample_cmd_source_filepaths)

add_executable(cxsample_cmd
        ${gs_cxsample_cmd_source_filepaths}
        )

set_target_properties(cxsample_cmd PROPERTIES FOLDER ccxx/sample)

add_dependencies(cxsample_cmd
        ccxx
        ccxx_database_odbc
        ccxx_database_sqlite
        )

target_link_libraries(cxsample_cmd
        ccxx
        ccxx_database_odbc
        ccxx_database_sqlite
        )


