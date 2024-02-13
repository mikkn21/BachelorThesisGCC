#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Integer
#include <boost/test/unit_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include <sstream>
#include "../src/parser/ast.hpp"
#include "../src/parser/grammar.hpp"


// Just a simple test to make sure the testing framework works

using boost::spirit::x3::ascii::space;
using boost::spirit::x3::phrase_parse;

void test_success(std::string input, bool expected_result) {
    grammar::ast::BinopExp result;
    auto it = input.begin();
    bool success = phrase_parse(it, input.end(), grammar::BinopExp(), space, result);


    std::ostringstream oss;
    oss << result; // Assuming operator<< is correctly overloaded for BinopExp
    std::string result_str = oss.str(); // Get the string from the ostringstream
    if (expected_result) {
        BOOST_CHECK(success && (it == input.end()) && input == result_str);
    } else {
        BOOST_CHECK(!(success && (it == input.end()) && input == result_str));
    }
}

BOOST_AUTO_TEST_CASE(BinopOperation)
{
    test_success("10+10", true);
}

