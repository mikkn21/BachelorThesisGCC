#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Integer
#include <boost/test/unit_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include "../src/ast.hpp"
#include <sstream>
#include "../src/parser/parser.hpp"
#include "../src/compiler.hpp"
#include <string>


enum class TestingOutcome {
    SUCCESS,
    PARSE1_FAILED,
    PARSE2_FAILED,
    PARSE1_INCONSISTENT,
    PARSE2_INCONSISTENT,
};



template<typename Compiler>
void test_parse(std::string input, TestingOutcome testing_outcome, Compiler compiler) {
    std::cout << std::endl;
    grammar::compiler::CompilerOptions options = grammar::compiler::CompilerOptions();
    options.stopAfter = grammar::compiler::StopAfterParser;
    options.printAst = true;
    options.printInput = true;

    grammar::ast::Prog ast1;
    try {
        ast1 = compiler(input, options).ast; // should call compile
    } catch (const grammar::parser::SyntaxError &e) {   
        std::ostringstream temp;
        temp << ast1;
        std::string ast1_string = temp.str();
        BOOST_CHECK_MESSAGE(testing_outcome == TestingOutcome::PARSE1_FAILED, "\nAST:\n" << ast1_string << "\n" <<  e.what() << "\n");
        return;
    }
    std::ostringstream temp1;
    temp1 << ast1;
    std::string ast1_string = temp1.str();

    grammar::ast::Prog ast2;
    try {
        ast2 = compiler(ast1_string, options).ast; // should call compile
    } catch (const grammar::parser::SyntaxError &e) {   
        std::ostringstream temp;
        temp << ast2;
        std::string ast2_string = temp.str();
        BOOST_CHECK_MESSAGE(testing_outcome == TestingOutcome::PARSE2_FAILED, "\nAST:\n" << ast1_string << "\n" << "AST:\n" << ast2_string << "\n" << e.what() << "\n");
        return;
    }
    std::ostringstream temp2;
    temp2 << ast2;
    std::string ast2_string = temp2.str();

    switch (testing_outcome) {
        case TestingOutcome::SUCCESS:
            BOOST_CHECK_MESSAGE(input == ast1_string, "\nParse1 output was not consistent with input, when it was expected\n");
            BOOST_CHECK_MESSAGE(ast1_string == ast2_string, "\nParse2 output was not consistent with Parse1 output, when it was expected\n");
            break;
        case TestingOutcome::PARSE1_INCONSISTENT:
            BOOST_CHECK_MESSAGE(input != ast1_string, "\nAST 1 output was consistent with the input, when it was expected not to\n");
            break;
        case TestingOutcome::PARSE2_INCONSISTENT:
            BOOST_CHECK_MESSAGE(ast1_string != ast2_string, "\nAST 2 output was consistent with the AST 1 output, when it was expected not to\n");   
            break;  
        case TestingOutcome::PARSE1_FAILED:
            BOOST_CHECK_MESSAGE(false, "\nParse1 did not fail, when it was expected to\n");
            break;
        case TestingOutcome::PARSE2_FAILED:
            BOOST_CHECK_MESSAGE(false, "\nParse2 did not fail, when it was expected to\n");
            break;
    }
}

void test_parse_file(std::string input, TestingOutcome testing_outcome) {
    test_parse(input, testing_outcome, grammar::compiler::compileFromFile);
}

void test_parse_string(std::string input, TestingOutcome testing_outcome) {
    test_parse(input, testing_outcome, grammar::compiler::compileFromString);
}

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
BOOST_AUTO_TEST_CASE(BinopOperationInvalidOperator) {test_parse_string("int x = 10 ? 10;", TestingOutcome::PARSE1_FAILED);}
BOOST_AUTO_TEST_CASE(BinopOperationMultipleBinaryOperators) {test_parse_string("int x = 10 + 10 * 10 / 10;", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationInvalidNoBinaryOperator) {test_parse_string("int x = 10  10;", TestingOutcome::PARSE1_FAILED);}
// BOOST_AUTO_TEST_CASE(prog1) {test_parse_file("../tests/parserTests/prog1.chad", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(prog2) {test_parse_file("../tests/parserTests/prog2.chad", TestingOutcome::SUCCESS);}