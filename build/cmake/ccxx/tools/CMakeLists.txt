
add_subdirectory(encoding)
add_subdirectory(filetail)
if (WIN32)
    add_subdirectory(daemon)
endif ()
add_subdirectory(openssl)
