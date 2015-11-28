
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/re2/filtered_re2.h \
    $$PWD/re2/prefilter.h \
    $$PWD/re2/prefilter_tree.h \
    $$PWD/re2/prog.h \
    $$PWD/re2/re2.h \
    $$PWD/re2/regexp.h \
    $$PWD/re2/set.h \
    $$PWD/re2/stringpiece.h \
    $$PWD/re2/unicode_casefold.h \
    $$PWD/re2/unicode_groups.h \
    $$PWD/re2/variadic_function.h \
    $$PWD/re2/walker-inl.h \
    $$PWD/util/atomicops.h \
    $$PWD/util/flags.h \
    $$PWD/util/logging.h \
    $$PWD/util/mutex.h \
    $$PWD/util/sparse_array.h \
    $$PWD/util/sparse_set.h \
    $$PWD/util/utf.h \
    $$PWD/util/util.h \
    $$PWD/util/valgrind.h \
    $$PWD/util/pcre.h \
    $$PWD/util/random.h

SOURCES += \
    $$PWD/re2/bitstate.cc \
    $$PWD/re2/compile.cc \
    $$PWD/re2/dfa.cc \
    $$PWD/re2/filtered_re2.cc \
    $$PWD/re2/mimics_pcre.cc \
    $$PWD/re2/nfa.cc \
    $$PWD/re2/onepass.cc \
    $$PWD/re2/parse.cc \
    $$PWD/re2/perl_groups.cc \
    $$PWD/re2/prefilter.cc \
    $$PWD/re2/prefilter_tree.cc \
    $$PWD/re2/prog.cc \
    $$PWD/re2/re2.cc \
    $$PWD/re2/regexp.cc \
    $$PWD/re2/set.cc \
    $$PWD/re2/simplify.cc \
    $$PWD/re2/stringpiece.cc \
    $$PWD/re2/tostring.cc \
    $$PWD/re2/unicode_casefold.cc \
    $$PWD/re2/unicode_groups.cc \
    $$PWD/util/hash.cc \
    $$PWD/util/rune.cc \
    $$PWD/util/stringprintf.cc \
    $$PWD/util/strutil.cc \
    $$PWD/util/valgrind.cc \
    $$PWD/util/pcre.cc \
    $$PWD/util/random.cc
