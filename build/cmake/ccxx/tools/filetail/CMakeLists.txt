
###has ccxx
set(fslSourceCcxxs
        filetail/main.cpp
        )

mc_merge_file_path(${gs_ccxx_tools_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtool_filetail
        ${fslSourceCcxxPaths})

add_dependencies(cxtool_filetail
        ccxx
        )

target_link_libraries(cxtool_filetail
        ccxx
        )

set_target_properties(cxtool_filetail PROPERTIES FOLDER ccxx/tools)



