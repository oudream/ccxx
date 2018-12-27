
add_subdirectory(./libuv)
add_subdirectory(./re2)
if (NOT WIN32)
    add_subdirectory(./htop)
endif ()
