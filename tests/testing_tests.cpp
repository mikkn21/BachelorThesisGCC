#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Integer
#include <boost/test/unit_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include "../src/parser/ast.hpp"
#include <sstream>
#include "../src/parser/parser.hpp"
#include <string>


enum class TestingOutcome {
    SUCCESS,
    PARSE1_FAILED,
    PARSE2_FAILED,
    PARSE1_INCONSISTENT,
    PARSE2_INCONSISTENT,
};

template <typename T>
void test_compile_string(std::string input, TestingOutcome testing_outcome, T exception) {
    try {
        grammar::ast::Prog ast1 = grammar::parser::parse(input); // should call compile
    } catch (T exception) {
        std::cout << exception.what() << endl;
    }
    std::ostringstream temp;
    temp << ast1;
    std::string ast1_string = temp.str();
    grammar::ast::Prog ast2 = grammar::parser::parse(ast1_string); // should call compile not parse
    

    // grammar::ast::Prog result1; // TODO remove. Should not be of type BinopExp, should be type Prog
    // auto input_begin = input.begin();

    // bool success1 = phrase_parse(input_begin, input.end(), grammar::parser::prog, space, result1);
    
    // std::ostringstream oss1;
    // oss1 << result1; 
    // std::string result1_str = oss1.str();
    // auto result1_str_begin = result1_str.begin();

    // grammar::ast::Prog result2; // TODO remove. Should not be of type BinopExp, should be type Prog
    // bool success2 = phrase_parse(result1_str_begin, result1_str.end(), grammar::parser::prog, space, result2);

    // std::ostringstream oss2;
    // oss2 << result2; 
    // std::string result2_str = oss2.str();

    // std::ostringstream message;
    // auto parse1_success = (success1 && (input_begin == input.end()) && input == result1_str);
    // auto parse2_success = (success2 && (result1_str_begin == result1_str.end()) && result1_str == result2_str);
    
    // switch(testing_outcome) {
    //     case TestingOutcome::SUCCESS:
    //         message << "Failed parsing. Here is a list of what happened, where if they print false, it failed:\n"
    //                 << "parse1 succeeded: " << (success1 ? "true" : "false") << "\n"
    //                 << "parse2 succeeded: " << (success2 ? "true" : "false") << "\n"
    //                 << "did parse1 parse the entire line?: " << ((input_begin == input.end()) ? "true" : "false") << "\n"
    //                 << "did parse2 parse the entire line?: " << ((result1_str_begin == result1_str.end()) ? "true" : "false") << "\n"
    //                 << "Was parse1 consistent?: " << (input == result1_str ? "true" : "false") << "\n"
    //                 << "Was parse2 consistent?: " << (success1 ? "true" : "false") << "\n\n"
    //                 << "parse1 was run on \"" << input << "\"\n"
    //                 << "parse1 outputted \"" << result1_str << "\"\n"
    //                 << "parse2 outputted \"" << result2_str << "\"\n";
    //         BOOST_CHECK_MESSAGE(parse1_success && parse2_success, message.str());
    //         break;      
    //     case TestingOutcome::PARSE1_FAILED:
    //         message << "First parse succeeded when failure was expected."
    //                 << "parse1 was run on \"" << input << "\"\n"
    //                 << "parse1 outputted \"" << result1_str << "\"\n";
    //         BOOST_CHECK_MESSAGE(!success1, message.str());
    //         break;
    //     case TestingOutcome::PARSE2_FAILED:
    //         BOOST_CHECK_MESSAGE(!success2, "Second parse succeeded when failure was expected.");
    //         break;
    //     case TestingOutcome::PARSE1_PARTIAL_SUCCESS:
    //         BOOST_CHECK_MESSAGE(success1 && (input_begin != input.end()), "First parse could only partially parse the input");
    //         break;
    //     case TestingOutcome::PARSE2_PARTIAL_SUCCESS:
    //         BOOST_CHECK_MESSAGE(success2 && (result1_str_begin != result1_str.end()), "First parse could only partially parse the input");
    //         break;
    //     case TestingOutcome::PARSE1_INCONSISTENT:
    //         BOOST_CHECK_MESSAGE(input != result1_str, "Parse2 output was consistent with the input, when it was expected not to");
    //         break;
    //     case TestingOutcome::PARSE2_INCONSISTENT:
    //         BOOST_CHECK_MESSAGE(result1_str != result2_str, "Parse2 output was consistent with the input, when it was expected not to");
    //         break;
    //     default:
    //         std::cout << "Unknown testing error";
    // };
}

// BOOST_AUTO_TEST_CASE(BinopOperationPlus) {test_parse("int x = 10 + 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationMinus) {test_parse("int x = 10 - 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationMultiplication) {test_parse("int x = 10 * 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationDivision) {test_parse("int x = 10 / 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationModulos) {test_parse("int x = 10 % 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationAnd) {test_parse("int x = 10 & 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationOr) {test_parse("int x = 10 | 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationNotEqual) {test_parse("int x = 10 != 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationEqual) {test_parse("int x = 10 == 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationGreater) {test_parse("int x = 10 > 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationGreaterEqual) {test_parse("int x = 10 >= 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationLesser) {test_parse("int x = 10 < 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationLesserEqual) {test_parse("int x = 10 <= 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationInvalidOperator) {test_parse("int x = 10 ? 10;", TestingOutcome::PARSE1_FAILED);}
// BOOST_AUTO_TEST_CASE(BinopOperationMultipleBinaryOperators) {test_parse("int x = 10 + 10 * 10 / 10;", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(BinopOperationInvalidNoBinaryOperator) {test_parse("int x = 10  10;", TestingOutcome::PARSE1_FAILED);}