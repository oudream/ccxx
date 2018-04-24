
mc_ccxx_sqlite_include()

add_definitions(-D"GM_BUILD_CCXX_SQLITE_LIB")

add_library(ccxx_database_sqlite SHARED
        ${gs_ccxx_sqlite_filepaths}
        )

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

add_dependencies(ccxx_database_sqlite
        ccxx)

target_link_libraries(ccxx_database_sqlite
        ccxx)

set_target_properties(ccxx_database_sqlite PROPERTIES FOLDER ccxx/lib)

#mc_post_build_copyfile(ccxx_database_sqlite)


