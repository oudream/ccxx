
include($$PWD/../../ccxx_qt_projects.pri)


TARGET = test_regexp

SOURCES += \
#    main.cpp \
#    test_regexp1.cpp \
#    test_regexp2.cpp \
    backtrack.cc \
    charclass_test.cc \
    compile_test.cc \
    dfa_test.cc \
    dump.cc \
    exhaustive_test.cc \
    exhaustive_tester.cc \
    exhaustive1_test.cc \
    exhaustive2_test.cc \
    exhaustive3_test.cc \
    filtered_re2_test.cc \
    mimics_pcre_test.cc \
    null_walker.cc \
    parse_test.cc \
    possible_match_test.cc \
    random_test.cc \
    re2_arg_test.cc \
    re2_test.cc \
    regexp_benchmark.cc \
    regexp_generator.cc \
    regexp_test.cc \
    required_prefix_test.cc \
    search_test.cc \
    set_test.cc \
    simplify_test.cc \
    string_generator.cc \
    string_generator_test.cc \
    tester.cc \
    $$PWD/../../ccxx/regexp/util/test.cc \

HEADERS += \
    $$PWD/../../ccxx/regexp/util/test.h \
