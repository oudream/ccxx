
include($$PWD/../../ccxx/ccxx_environment.pri)
include($$CCXX_PATH/ccxx/googletest/googletest.pri)

INCLUDEPATH += $$PWD/../../ccxx/googletest

TARGET = test_googletest

HEADERS += \
    $$PWD/samples/prime_tables.h \
    $$PWD/samples/sample1.h \
    $$PWD/samples/sample2.h \
    $$PWD/samples/sample3-inl.h \
    $$PWD/samples/sample4.h \
    $$PWD/test/gtest-param-test_test.h \
    $$PWD/test/gtest-typed-test_test.h \
    $$PWD/test/production.h

SOURCES += \
    $$PWD/../../ccxx/googletest/src/gtest_main.cc\
    $$PWD/samples/sample1.cc \
    $$PWD/samples/sample1_unittest.cc \
#    $$PWD/samples/sample2.cc \
#    $$PWD/samples/sample2_unittest.cc \
#    $$PWD/samples/sample3_unittest.cc \
#    $$PWD/samples/sample4.cc \
#    $$PWD/samples/sample5_unittest.cc \
#    $$PWD/samples/sample6_unittest.cc \
#    $$PWD/samples/sample7_unittest.cc \
#    $$PWD/samples/sample8_unittest.cc \
#    $$PWD/samples/sample9_unittest.cc \
#    $$PWD/samples/sample10_unittest.cc \
#    $$PWD/test/gtest_all_test.cc \
#    $$PWD/test/gtest_break_on_failure_unittest_.cc \
#    $$PWD/test/gtest_catch_exceptions_test_.cc \
#    $$PWD/test/gtest_color_test_.cc \
#    $$PWD/test/gtest_env_var_test_.cc \
#    $$PWD/test/gtest_environment_test.cc \
#    $$PWD/test/gtest_filter_unittest_.cc \
#    $$PWD/test/gtest_help_test_.cc \
#    $$PWD/test/gtest_list_tests_unittest_.cc \
#    $$PWD/test/gtest_main_unittest.cc \
#    $$PWD/test/gtest_no_test_unittest.cc \
#    $$PWD/test/gtest_output_test_.cc \
#    $$PWD/test/gtest_pred_impl_unittest.cc \
#    $$PWD/test/gtest_premature_exit_test.cc \
#    $$PWD/test/gtest_prod_test.cc \
#    $$PWD/test/gtest_repeat_test.cc \
#    $$PWD/test/gtest_shuffle_test_.cc \
#    $$PWD/test/gtest_sole_header_test.cc \
#    $$PWD/test/gtest_stress_test.cc \
#    $$PWD/test/gtest_throw_on_failure_ex_test.cc \
#    $$PWD/test/gtest_throw_on_failure_test_.cc \
#    $$PWD/test/gtest_uninitialized_test_.cc \
#    $$PWD/test/gtest_unittest.cc \
#    $$PWD/test/gtest_xml_outfile1_test_.cc \
#    $$PWD/test/gtest_xml_outfile2_test_.cc \
#    $$PWD/test/gtest_xml_output_unittest_.cc \
#    $$PWD/test/gtest-death-test_ex_test.cc \
#    $$PWD/test/gtest-death-test_test.cc \
#    $$PWD/test/gtest-filepath_test.cc \
#    $$PWD/test/gtest-linked_ptr_test.cc \
#    $$PWD/test/gtest-listener_test.cc \
#    $$PWD/test/gtest-message_test.cc \
#    $$PWD/test/gtest-options_test.cc \
#    $$PWD/test/gtest-param-test_test.cc \
#    $$PWD/test/gtest-param-test2_test.cc \
#    $$PWD/test/gtest-port_test.cc \
#    $$PWD/test/gtest-printers_test.cc \
#    $$PWD/test/gtest-test-part_test.cc \
#### $$PWD/test/gtest-tuple_test.cc \
#    $$PWD/test/gtest-typed-test_test.cc \
#    $$PWD/test/gtest-typed-test2_test.cc \
#    $$PWD/test/gtest-unittest-api_test.cc \
#### $$PWD/test/production.cc \


DISTFILES += \
    $$PWD/test/gtest_output_test_golden_lin.txt \
    $$PWD/test/gtest_break_on_failure_unittest.py \
    $$PWD/test/gtest_catch_exceptions_test.py \
    $$PWD/test/gtest_color_test.py \
    $$PWD/test/gtest_env_var_test.py \
    $$PWD/test/gtest_filter_unittest.py \
    $$PWD/test/gtest_help_test.py \
    $$PWD/test/gtest_list_tests_unittest.py \
    $$PWD/test/gtest_output_test.py \
    $$PWD/test/gtest_shuffle_test.py \
    $$PWD/test/gtest_test_utils.py \
    $$PWD/test/gtest_throw_on_failure_test.py \
    $$PWD/test/gtest_uninitialized_test.py \
    $$PWD/test/gtest_xml_outfiles_test.py \
    $$PWD/test/gtest_xml_output_unittest.py \
    $$PWD/test/gtest_xml_test_utils.py
