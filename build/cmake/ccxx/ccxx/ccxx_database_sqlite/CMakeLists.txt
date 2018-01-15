
mc_ccxx_sqlite_include()

add_definitions(-D"GM_BUILD_CCXX_SQLITE_LIB")

add_library(libccxx_database_sqlite SHARED
        ${gs_ccxx_sqlite_filepaths}
        )

if (MSVC)
    add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
    add_definitions( "/wd4819" )
endif ()

add_dependencies(libccxx_database_sqlite
        libccxx)

target_link_libraries(libccxx_database_sqlite
        libccxx)

set_target_properties(libccxx_database_sqlite PROPERTIES FOLDER ccxx/lib)

#mc_post_build_copyfile(libccxx_database_sqlite)


