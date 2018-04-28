
mc_ccxx_odbc_include()

add_definitions(-D"GM_BUILD_CCXX_ODBC_LIB")

add_library(ccxx_database_odbc SHARED
        ${gs_ccxx_odbc_filepaths}
        )

add_dependencies(ccxx_database_odbc
        ccxx
        )

target_link_libraries(ccxx_database_odbc
        ccxx
        )

if (WIN32)
    target_link_libraries(ccxx_database_odbc
            odbc32
            Ole32
            )
elseif(APPLE)
    add_definitions(-D"OTL_ODBC_UNIX")
    INCLUDE_DIRECTORIES(/usr/local/include)
    LINK_DIRECTORIES(/usr/local/lib)
    target_link_libraries(ccxx_database_odbc
            /usr/local/lib/libodbc.dylib
            )
else()
    add_definitions(-D"OTL_ODBC_UNIX")
    target_link_libraries(ccxx_database_odbc
            odbc
            )
endif ()

set_target_properties(ccxx_database_odbc PROPERTIES FOLDER ccxx/lib)

#mc_post_build_copyfile(ccxx_database_odbc)

