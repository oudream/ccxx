
set(gs_ccxx_sample_cmd_sources
        cmd/main.cpp
        )

mc_merge_file_path(${gs_ccxx_sample_path} "${gs_ccxx_sample_cmd_sources}" gs_ccxx_sample_cmd_source_filepaths)

add_executable(ccxx_sample_cmd
        ${gs_ccxx_sample_cmd_source_filepaths}
        )

set_target_properties(ccxx_sample_cmd PROPERTIES FOLDER ccxx/sample)

add_dependencies(ccxx_sample_cmd
        libccxx
        libccxx_database_odbc
        libccxx_database_sqlite
        )

target_link_libraries(ccxx_sample_cmd
        libccxx
        libccxx_database_odbc
        libccxx_database_sqlite
        )


