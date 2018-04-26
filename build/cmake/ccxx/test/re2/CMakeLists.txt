###environment

set(fslSources
        ###can not compile
        ###SOURCES += \
        ###    $$PWD/benchmark.cc \
        ###    $$PWD/regexp_benchmark.cc \
#        regexp/main.cpp
#        regexp/test_regexp.cpp
        regexp/../../ccxx/regexp/util/test.h
        regexp/../../ccxx/regexp/util/test.cc
        regexp/tester.cc
        regexp/backtrack.cc
        regexp/charclass_test.cc
        regexp/compile_test.cc
        regexp/dfa_test.cc
        regexp/dump.cc
        regexp/exhaustive_test.cc
        regexp/exhaustive_tester.cc
        regexp/exhaustive1_test.cc
        regexp/exhaustive2_test.cc
        regexp/exhaustive3_test.cc
        regexp/filtered_re2_test.cc
        regexp/mimics_pcre_test.cc
        regexp/null_walker.cc
        regexp/parse_test.cc
        regexp/possible_match_test.cc
        regexp/random_test.cc
        regexp/re2_arg_test.cc
        regexp/re2_test.cc
        regexp/regexp_generator.cc
        regexp/regexp_test.cc
        regexp/required_prefix_test.cc
        regexp/search_test.cc
        regexp/set_test.cc
        regexp/simplify_test.cc
        regexp/string_generator.cc
        regexp/string_generator_test.cc
        )

mc_merge_file_path(${gs_ccxx_test_path} "${fslSources}" fslSourcePaths)

include_directories(${gs_ccxx_path}/ccxx/regexp)

add_executable(cxtest_regexp
        ${fslSourcePaths}
        )

add_dependencies(cxtest_regexp
        ccxx
        )

target_link_libraries(cxtest_regexp
        ccxx
        )

set_target_properties(cxtest_regexp PROPERTIES FOLDER ccxx/test)






