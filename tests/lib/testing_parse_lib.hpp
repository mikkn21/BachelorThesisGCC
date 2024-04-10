#ifndef PARSER_TEST_LIB_HPP
#define PARSER_TEST_LIB_HPP

#include <boost/test/unit_test.hpp>

enum class TestingOutcome {
    SUCCESS,
    PARSE_FAILED,
    PARSE2_FAILED,
    PARSE1_INCONSISTENT,
    PARSE2_INCONSISTENT,
};

void test_parse_file(std::string input, TestingOutcome testing_outcome);
void test_parse_string(std::string input, TestingOutcome testing_outcome);

#endif // PARSER_TEST_LIB_HPP
