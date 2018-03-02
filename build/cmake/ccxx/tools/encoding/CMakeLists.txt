
###has ccxx
set(fslSourceCcxxs
        encoding/main.cpp
        )

mc_merge_file_path(${gs_ccxx_tools_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtool_encoding
        ${fslSourceCcxxPaths})

add_dependencies(cxtool_encoding
        libccxx
        )

target_link_libraries(cxtool_encoding
        libccxx
        )

set_target_properties(cxtool_encoding PROPERTIES FOLDER ccxx/tools)



