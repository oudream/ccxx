
mc_htop_gui_include()
mc_htop_main_include()

if (WIN32)
elseif (APPLE)
    mc_htop_darwin_include()
    set(gsl_htop_os_head_filepaths ${gsl_htop_darwin_head_filepaths})
    set(gsl_htop_os_source_filepaths ${gsl_htop_darwin_source_filepaths})
else()
    mc_htop_linux_include()
    set(gsl_htop_os_head_filepaths ${gsl_htop_linux_head_filepaths})
    set(gsl_htop_os_source_filepaths ${gsl_htop_linux_source_filepaths})
endif ()

add_executable(htop_main
        ${gsl_htop_main_head_filepaths} ${gsl_htop_main_source_filepaths}
        ${gsl_htop_gui_head_filepaths} ${gsl_htop_gui_source_filepaths}
        ${gsl_htop_header_filepaths} ${gsl_htop_source_filepaths}
        ${gsl_htop_os_head_filepaths} ${gsl_htop_os_source_filepaths}
        )
#
#add_dependencies(htop_main
#        htop_dyl
#        )
#
#target_link_libraries(htop_main
#        htop_dyl
#        )

#add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /nologo" )

#add_definitions(-D"DEBUG")

mc_target_link_libraries(htop_main)

if (WIN32)
    if (MSVC)
        add_definitions(-D"_CRT_SECURE_NO_WARNINGS")
        add_definitions( "/wd4819" )
    endif ()
elseif (APPLE)
    #otool -L /usr/local/bin/htop
    #/usr/local/bin/htop:
    #/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit (compatibility version 1.0.0, current version 275.0.0)
    #/System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation (compatibility version 150.0.0, current version 1443.13.0)
    #/usr/lib/libncurses.5.4.dylib (compatibility version 5.4.0, current version 5.4.0)
    #/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1252.0.0)

    find_library(IOKIT_LIBRARY IOKit)
    target_link_libraries(htop_main
            ${IOKIT_LIBRARY}
            ncurses
            )
else()
    #ldd /usr/bin/htop
    #linux-vdso.so.1 =>  (0x00007fff93feb000)
    #libncursesw.so.5 => /lib/x86_64-linux-gnu/libncursesw.so.5 (0x00007faefd065000)
    #libtinfo.so.5 => /lib/x86_64-linux-gnu/libtinfo.so.5 (0x00007faefce3c000)
    #libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007faefcb33000)
    #libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007faefc769000)
    #libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007faefc565000)
    #/lib64/ld-linux-x86-64.so.2 (0x00007faefd4c1000)

    target_link_libraries(htop_main
            ncurses
            ncursesw
            tinfo
            m
            )
endif ()

set_target_properties(htop_main PROPERTIES FOLDER 3rd/htop)
