
add_subdirectory(ccxx_lib)
add_subdirectory(ccxx_database_sqlite)
add_subdirectory(ccxx_database_odbc)
if(${gs_project_enable_curl})
    add_subdirectory(ccxx_curl)
endif()