#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Integer
#include <boost/test/unit_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include <sstream>
#include "../src/parser/ast.hpp"
// #include "../src/parser/grammar.hpp"


// Just a simple test to make sure the testing framework works

using boost::spirit::x3::ascii::space;
using boost::spirit::x3::phrase_parse;

void test(std::string input, bool expected_result) {
    grammar::ast::BinopExp result; // TODO remove. Should not be of type BinopExp, should be type Prog
    auto input_begin = input.begin();
    bool success = phrase_parse(input_begin, input.end(), grammar::BinopExp(), space, result);

    std::ostringstream oss;
    oss << result; // Assuming operator<< is correctly overloaded for BinopExp
    std::string result_str = oss.str(); // Get the string from the ostringstream
    auto result_str_begin = result_str.begin();

    grammar::ast::BinopExp stabilized_result; // TODO remove. Should not be of type BinopExp, should be type Prog
    bool stabibilized_success = phrase_parse(result_str_begin, result_str.end(), grammar::BinopExp(), space, stabilized_result);

    
    parse1_success == (success && (input_begin == input.end()) && input == result_str);
    if (expected_result) {
        BOOST_CHECK(success && (input_begin == input.end()) && (result_str_begin == result_str.end()) && stabibilized_success && input == result_str);
    } else {
        BOOST_CHECK(!);
    }
}

BOOST_AUTO_TEST_CASE(BinopOperationPlus) {test("10 + 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationMinus) {test("10 - 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationMultiplication) {test("10 * 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationDivision) {test("10 / 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationModulos) {test("10 % 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationAnd) {test("10 & 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationOr) {test("10 | 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationNotEqual) {test("10 != 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationEqual) {test("10 == 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationGreater) {test("10 > 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationGreaterEqual) {test("10 >= 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationLesser) {test("10 < 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationLesserEqual) {test("10 <= 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationInvalidOperator) {test("10 ? 10", false);}
BOOST_AUTO_TEST_CASE(BinopOperationMultipleBinaryOperators) {test("10 + 10 * 10 / 10", true);}
BOOST_AUTO_TEST_CASE(BinopOperationInvalidNoBinaryOperator) {test("10  10", false);}






