
###has ccxx
set(fslSourceCcxxs
        redist-simple/main.cpp
        )

mc_merge_file_path(${gs_hiredis_cxtest_path} "${fslSourceCcxxs}" fslSourceCcxxPaths)

add_executable(cxtest_hiredis_redis_simple
        ${fslSourceCcxxPaths})

add_dependencies(cxtest_hiredis_redis_simple
        ccxx
        hiredis
        )

target_link_libraries(cxtest_hiredis_redis_simple
        ccxx
        hiredis
        )

set_target_properties(cxtest_hiredis_redis_simple PROPERTIES FOLDER ccxx/test/hiredis)
