
mc_target_define_qt()

set(SOURCES
        qtui1/main.cpp
        qtui1/mainwindow.cpp
        qtui1/test_qobject.cpp
        )

set(HEADERS
        qtui1/mainwindow.h
        qtui1/test_qobject.h
        )

set(FORMS
        qtui1/mainwindow.ui
        )

mc_merge_file_path(${gs_ccxx_test_env_path} "${SOURCES}" fslSOURCESPaths)
mc_merge_file_path(${gs_ccxx_test_env_path} "${HEADERS}" fslHEADERSPaths)
mc_merge_file_path(${gs_ccxx_test_env_path} "${FORMS}" fslFORMSPaths)


add_executable(test_env_qtui1
        ${fslSOURCESPaths}
        ${fslHEADERSPaths}
        ${fslFORMSPaths}
        )

set_target_properties(test_env_qtui1 PROPERTIES FOLDER ccxx/test)

target_link_libraries(test_env_qtui1
        ${Qt5Core_LIBRARIES}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Gui_LIBRARIES}
        )

###copyfile is success
#if(MSVC)
#    add_custom_command(TARGET test_env_qtui1 POST_BUILD
#            COMMAND python "C:/ddd/cpy/test/hello.py" $(Configuration)
#            )
#else()
#    add_custom_command(TARGET test_env_qtui1 POST_BUILD
#            COMMAND python "${gs_project_ccpro_script_path}/python/deploy_copyfile.py" -ct ${CMAKE_BUILD_TYPE} -sf test_env_qtui1
#            )
#endif()
