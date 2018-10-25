
set(gs_cxsample_lua_sources
        lua/main.cpp
        )

mc_merge_file_path(${gs_ccxx_sample_path} "${gs_cxsample_lua_sources}" gs_cxsample_lua_source_filepaths)

add_executable(cxsample_lua
        ${gs_cxsample_lua_source_filepaths}
        )

set_target_properties(cxsample_lua PROPERTIES FOLDER ccxx/sample)

add_dependencies(cxsample_lua
        ccxx
        ccxx_database_odbc
        ccxx_database_sqlite
        cxlua
        )

target_link_libraries(cxsample_lua
        ccxx
        ccxx_database_odbc
        ccxx_database_sqlite
        cxlua
        )


