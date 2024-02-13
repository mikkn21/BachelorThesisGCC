#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Integer
#include <boost/test/unit_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include "../src/parser/ast.cpp"
#include <sstream>

using boost::spirit::x3::ascii::space;
using boost::spirit::x3::phrase_parse;

void test(std::string input, bool expected_result) {
    grammar::ast::Prog result1; // TODO remove. Should not be of type BinopExp, should be type Prog
    auto input_begin = input.begin();
    bool success1 = phrase_parse(input_begin, input.end(), grammar::ast::Prog(), space, result1);

    std::ostringstream oss1;
    oss1 << result1; 
    std::string result1_str = oss1.str();
    auto result1_str_begin = result1_str.begin();

    grammar::ast::BinopExp result2; // TODO remove. Should not be of type BinopExp, should be type Prog
    bool success2 = phrase_parse(result1_str_begin, result1_str.end(), grammar::ast::BinopExp(), space, result2);

    std::ostringstream oss2;
    oss2 << result2; 
    std::string result2_str = oss2.str();

    BOOST_REQUIRE_MESSAGE(result1_str == result2_str, "Parser AST did not stabililize");
    
    auto parse1_success = (success1 && (input_begin == input.end()) && input == result1_str);
    auto parse2_success = (success2 && (result1_str_begin == result1_str.end()) && result1_str == result2_str);
    if (expected_result) {
        BOOST_CHECK(parse1_success && parse2_success);
    } else {
        BOOST_CHECK(!(parse1_success && parse2_success));
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
