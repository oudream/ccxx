
add_subdirectory(cmd)
add_subdirectory(env)
add_subdirectory(interinfo)
add_subdirectory(simple)
if (${gs_project_enable_qt})
    add_subdirectory(qt)
endif ()
