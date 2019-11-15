
include(htop.cmake)

mc_htop_generate_config()
mc_htop_include()

add_subdirectory(dll)
add_subdirectory(main)
add_subdirectory(ccxx)

