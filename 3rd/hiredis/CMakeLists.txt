
if (${gs_project_enable_hiredis})
    if (MSVC)
        add_subdirectory(./hiredis-msvc)
    else()
        add_subdirectory(./hiredis)
    endif()
endif()
