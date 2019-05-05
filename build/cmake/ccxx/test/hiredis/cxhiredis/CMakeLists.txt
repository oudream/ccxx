
###has ccxx
set(fslSourceCcxxs
        cxhiredis/main.cpp
        )

mc_merge_file_path(${gs_hiredis_cxtest_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

mc_ccxx_hiredis_include()

add_executable(cxtest_hiredis_cxhiredis
        ${fslSourceCcxxPaths}
        ${gsl_ccxx_hiredis_filepaths}
        )

add_dependencies(cxtest_hiredis_cxhiredis
        ccxx
        hiredis
        )

target_link_libraries(cxtest_hiredis_cxhiredis
        ccxx
        hiredis
        )

set_target_properties(cxtest_hiredis_cxhiredis PROPERTIES FOLDER ccxx/test/hiredis)
