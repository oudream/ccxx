

###qt4
if (DEFINED gs_project_qt_version AND (gs_project_qt_version LESS_EQUAL 5))
    find_package(Qt4 REQUIRED)

    include(${QT_USE_FILE})

    add_definitions(${QT_DEFINITIONS})
else()
    ###qt5
    find_package(Qt5Core    REQUIRED)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Gui     REQUIRED)
    find_package(Qt5Svg     REQUIRED)
    find_package(Qt5Network REQUIRED)
endif ()

####do not use qtgui
##set(QT_DONT_USE_QTGUI TRUE)

###
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find includes in corresponding build directories
# CMake automatically adds CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR to the include path for each directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#
