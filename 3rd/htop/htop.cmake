

#1, htop
set(gs_htop_path ${CMAKE_CURRENT_SOURCE_DIR})
set(gs_htop_include_path ${gs_htop_path})


set(gsl_htop_headers
        htop/RichString.h
        htop/XAlloc.h
        htop/Object.h
        htop/Hashtable.h
        htop/FunctionBar.h
        htop/Process.h
        htop/Vector.h
        htop/UsersTable.h
        htop/Panel.h
        htop/ProcessList.h
        htop/Settings.h
        htop/CRT.h
        htop/ListItem.h
        htop/StringUtils.h
        htop/Meter.h
        htop/CPUMeter.h
        htop/UptimeMeter.h
        htop/AvailableMetersPanel.h
        htop/BatteryMeter.h
        htop/ClockMeter.h
        htop/HostnameMeter.h
        htop/LoadAverageMeter.h
        htop/MemoryMeter.h
        htop/MetersPanel.h
        htop/SwapMeter.h
        htop/TasksMeter.h
        htop/ListItem.h
        htop/Header.h
        htop/ScreenManager.h
        )

set(gsl_htop_sources
        htop/RichString.c
        htop/XAlloc.c
        htop/Object.c
        htop/Hashtable.c
        htop/FunctionBar.c
        htop/Process.c
        htop/Vector.c
        htop/UsersTable.c
        htop/Panel.c
        htop/ProcessList.c
        htop/Settings.c
        htop/CRT.c
        htop/ListItem.c
        htop/StringUtils.c
        htop/Meter.c
        htop/CPUMeter.c
        htop/UptimeMeter.c
        htop/AvailableMetersPanel.c
        htop/BatteryMeter.c
        htop/ClockMeter.c
        htop/HostnameMeter.c
        htop/LoadAverageMeter.c
        htop/MemoryMeter.c
        htop/MetersPanel.c
        htop/SwapMeter.c
        htop/TasksMeter.c
        htop/ListItem.h
        htop/Header.c
        htop/ScreenManager.c
        )


mc_merge_file_path(${gs_htop_path} "${gsl_htop_headers}" gsl_htop_header_filepaths)
mc_merge_file_path(${gs_htop_path} "${gsl_htop_sources}" gsl_htop_source_filepaths)

macro(mc_htop_include)
    include_directories(${gs_htop_include_path})
    source_group("htop" FILES ${gsl_htop_header_filepaths} ${gsl_htop_source_filepaths})
endmacro()



#2, htop_os_darwin
set(gsl_htop_darwin_heads
        htop/darwin/DarwinCRT.h
        htop/darwin/DarwinProcess.h
        htop/darwin/DarwinProcessList.h
        htop/darwin/Platform.h
        htop/darwin/Battery.h
        )

set(gsl_htop_darwin_sources
        htop/darwin/DarwinCRT.c
        htop/darwin/DarwinProcess.c
        htop/darwin/DarwinProcessList.c
        htop/darwin/Platform.c
        htop/darwin/Battery.c
        )

mc_merge_file_path(${gs_htop_path} "${gsl_htop_darwin_heads}" gsl_htop_darwin_head_filepaths)
mc_merge_file_path(${gs_htop_path} "${gsl_htop_darwin_sources}" gsl_htop_darwin_source_filepaths)

set(gs_htop_darwin_path ${gs_htop_path}/htop/darwin)

macro(mc_htop_darwin_include)
    include_directories(${gs_htop_path}/htop)
    include_directories(${gs_htop_darwin_path})
    source_group("htop_darwin" FILES ${gsl_htop_darwin_head_filepaths} ${gsl_htop_darwin_source_filepaths})
endmacro()


#2, htop_os_linux
set(gsl_htop_linux_heads
        htop/linux/Battery.h
        htop/linux/IOPriority.h
        htop/linux/IOPriorityPanel.h
        htop/linux/LinuxCRT.h
        htop/linux/LinuxProcess.h
        htop/linux/LinuxProcessList.h
        htop/linux/Platform.h
        )

set(gsl_htop_linux_sources
        htop/linux/Battery.c
        htop/linux/IOPriority.c
        htop/linux/IOPriorityPanel.c
        htop/linux/LinuxCRT.c
        htop/linux/LinuxProcess.c
        htop/linux/LinuxProcessList.c
        htop/linux/Platform.c
        )

mc_merge_file_path(${gs_htop_path} "${gsl_htop_linux_heads}" gsl_htop_linux_head_filepaths)
mc_merge_file_path(${gs_htop_path} "${gsl_htop_linux_sources}" gsl_htop_linux_source_filepaths)

set(gs_htop_linux_path ${gs_htop_path}/htop/linux)

macro(mc_htop_linux_include)
    include_directories(${gs_htop_path}/htop)
    include_directories(${gs_htop_linux_path})
    source_group("htop_linux" FILES ${gsl_htop_linux_head_filepaths} ${gsl_htop_linux_source_filepaths})
endmacro()


#3, htop_gui
set(gsl_htop_gui_heads
        htop/ColumnsPanel.h
        htop/MainPanel.h
        htop/Action.h
        htop/Affinity.h
        htop/AffinityPanel.h
        htop/IncSet.h
        htop/CategoriesPanel.h
        htop/AvailableMetersPanel.h
        htop/DisplayOptionsPanel.h
        htop/ColorsPanel.h
        htop/AvailableColumnsPanel.h
        htop/CheckItem.h
        htop/EnvScreen.h
        htop/InfoScreen.h
        htop/OpenFilesScreen.h
        htop/SignalsPanel.h
        htop/TraceScreen.h
        )

set(gsl_htop_gui_sources
        htop/ColumnsPanel.c
        htop/MainPanel.c
        htop/Action.c
        htop/Affinity.c
        htop/AffinityPanel.c
        htop/IncSet.c
        htop/CategoriesPanel.c
        htop/AvailableMetersPanel.c
        htop/DisplayOptionsPanel.c
        htop/ColorsPanel.c
        htop/AvailableColumnsPanel.c
        htop/CheckItem.c
        htop/EnvScreen.c
        htop/InfoScreen.c
        htop/OpenFilesScreen.c
        htop/SignalsPanel.c
        htop/TraceScreen.c
        )

mc_merge_file_path(${gs_htop_path} "${gsl_htop_gui_heads}" gsl_htop_gui_head_filepaths)
mc_merge_file_path(${gs_htop_path} "${gsl_htop_gui_sources}" gsl_htop_gui_source_filepaths)

macro(mc_htop_gui_include)
    source_group("htop_gui" FILES ${gsl_htop_gui_head_filepaths} ${gsl_htop_gui_source_filepaths})
endmacro()



#4, htop_main
set(gsl_htop_main_heads
        htop/htop.h
        )

set(gsl_htop_main_sources
        htop/htop.c
        )

mc_merge_file_path(${gs_htop_path} "${gsl_htop_main_heads}" gsl_htop_main_head_filepaths)
mc_merge_file_path(${gs_htop_path} "${gsl_htop_main_sources}" gsl_htop_main_source_filepaths)

macro(mc_htop_main_include)
    source_group("htop_main" FILES ${gsl_htop_main_head_filepaths} ${gsl_htop_main_source_filepaths})
endmacro()
