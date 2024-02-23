#include "testing_parse_lib.hpp"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Integer
#include <boost/test/unit_test.hpp>
#include <boost/spirit/home/x3.hpp>


BOOST_AUTO_TEST_CASE(BinopOperationPlus) {test_parse_string("int x = 10 + 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationMinus) {test_parse_string("int x = 10 - 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationMultiplication) {test_parse_string("int x = 10 * 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationDivision) {test_parse_string("int x = 10 / 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationModulos) {test_parse_string("int x = 10 % 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationAnd) {test_parse_string("int x = 10 & 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationOr) {test_parse_string("int x = 10 | 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationNotEqual) {test_parse_string("int x = 10 != 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationEqual) {test_parse_string("int x = 10 == 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationGreater) {test_parse_string("int x = 10 > 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationGreaterEqual) {test_parse_string("int x = 10 >= 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationLesser) {test_parse_string("int x = 10 < 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationLesserEqual) {test_parse_string("int x = 10 <= 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationInvalidOperator) {test_parse_string("int x = 10 ? 10;", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(BinopOperationMultipleBinaryOperators) {test_parse_string("int x = 10 + 10 * 10 / 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationInvalidNoBinaryOperator) {test_parse_string("int x = 10  10;", TestingOutcome::PARSE_FAILED);}
// BOOST_AUTO_TEST_CASE(prog1) {test_parse_file("../tests/parserTests/prog1.chad", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(prog2) {test_parse_file("../tests/parserTests/prog2.chad", TestingOutcome::SUCCESS);}