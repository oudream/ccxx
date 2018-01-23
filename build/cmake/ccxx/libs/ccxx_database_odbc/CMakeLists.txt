
mc_ccxx_odbc_include()

add_definitions(-D"GM_BUILD_CCXX_ODBC_LIB")

add_library(libccxx_database_odbc SHARED
        ${gs_ccxx_odbc_filepaths}
        )

add_dependencies(libccxx_database_odbc
        libccxx
        )

target_link_libraries(libccxx_database_odbc
        libccxx
        )

if (WIN32)
    target_link_libraries(libccxx_database_odbc
            odbc32
            Ole32
            )
else()
    add_definitions(-D"OTL_ODBC_UNIX")
    target_link_libraries(libccxx_database_odbc
            odbc
            )
endif ()

set_target_properties(libccxx_database_odbc PROPERTIES FOLDER ccxx/lib)

#mc_post_build_copyfile(libccxx_database_odbc)

