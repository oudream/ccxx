
###add qt ui
include(./../../../qt.cmake)

### ccxx qt
mc_ccxx_qt_include()

###
set(qt_sources
        qt/main.cpp
        qt/mainwindow.cpp
        )

set(qt_headers
        qt/mainwindow.h
        )

set(qt_forms
        qt/mainwindow.ui
        )

mc_merge_file_path(${gs_ccxx_sample_path} "${qt_sources}" qt_source_paths)
mc_merge_file_path(${gs_ccxx_sample_path} "${qt_headers}" qt_header_paths)
mc_merge_file_path(${gs_ccxx_sample_path} "${qt_forms}" qt_form_paths)

source_group("qt" FILES ${qt_source_paths} ${qt_header_paths} ${qt_form_paths})


###add ccxx
add_executable(ccxx_sample_qt
        ${qt_source_paths} ${qt_header_paths} ${qt_form_paths}
        ${gsl_ccxx_qt_filepaths}
        )

set_target_properties(ccxx_sample_qt PROPERTIES FOLDER ccxx/sample)

add_dependencies(ccxx_sample_qt
        libccxx
        libccxx_database_odbc
        libccxx_database_sqlite
        )

target_link_libraries(ccxx_sample_qt
        libccxx
        libccxx_database_odbc
        libccxx_database_sqlite
        ${Qt5Core_LIBRARIES}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Gui_LIBRARIES}
        )


