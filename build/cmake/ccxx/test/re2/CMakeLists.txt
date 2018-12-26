
set(gs_re2_path ${gs_project_path}/3rd/re2/re2)

set(gs_re2_src_path ${gs_re2_path})
set(gs_re2_include_path ${gs_re2_path})
set(gs_re2_cxtest_path ${gs_project_path}/test/re2)

include_directories(${gs_re2_include_path})

add_subdirectory(test_re2_1)
