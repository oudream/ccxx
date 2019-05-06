
add_subdirectory(encoding)
add_subdirectory(filetail)
if (WIN32)
    add_subdirectory(daemon)
endif ()
if (${gs_project_enable_openssl})
    add_subdirectory(openssl)
endif ()
