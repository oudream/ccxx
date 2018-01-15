

SOURCES += \
#    $$PWD/main.cpp \
#    $$PWD/test_regexp.cpp \

###can not compile
###SOURCES += \
###    $$PWD/benchmark.cc \
###    $$PWD/regexp_benchmark.cc \

HEADERS += \
    $$PWD/../../ccxx/regexp/util/test.h \

SOURCES += \
    $$PWD/../../ccxx/regexp/util/test.cc \
    $$PWD/tester.cc \
    $$PWD/backtrack.cc \
    $$PWD/charclass_test.cc \
    $$PWD/compile_test.cc \
    $$PWD/dfa_test.cc \
    $$PWD/dump.cc \
    $$PWD/exhaustive_test.cc \
    $$PWD/exhaustive_tester.cc \
    $$PWD/exhaustive1_test.cc \
    $$PWD/exhaustive2_test.cc \
    $$PWD/exhaustive3_test.cc \
    $$PWD/filtered_re2_test.cc \
    $$PWD/mimics_pcre_test.cc \
    $$PWD/null_walker.cc \
    $$PWD/parse_test.cc \
    $$PWD/possible_match_test.cc \
    $$PWD/random_test.cc \
    $$PWD/re2_arg_test.cc \
    $$PWD/re2_test.cc \
    $$PWD/regexp_generator.cc \
    $$PWD/regexp_test.cc \
    $$PWD/required_prefix_test.cc \
    $$PWD/search_test.cc \
    $$PWD/set_test.cc \
    $$PWD/simplify_test.cc \
    $$PWD/string_generator.cc \
    $$PWD/string_generator_test.cc \

