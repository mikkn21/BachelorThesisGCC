#include "../../src/ast.hpp"
#include <sstream>
#include "../../src/parser/parser.hpp"
#include "../../src/compiler.hpp"
#include <string>
#include "testing_parse_lib.hpp"

std::string remove_whitespace(const std::string& str) {
    std::string result;
    std::remove_copy_if(str.begin(), str.end(), 
                        std::back_inserter(result),
                        ::isspace); 
    return result;
}

bool equal_ignore_whitespaces(const std::string& str1, const std::string& str2) {
    return (remove_whitespace(str1) == remove_whitespace(str2));
}

template<typename Compiler>
void test_parse(std::string input, TestingOutcome testing_outcome, Compiler compiler) {
    std::cout << "\n----------------------" << std::endl;
    grammar::compiler::CompilerOptions options = grammar::compiler::CompilerOptions();
    options.stopAfter = grammar::compiler::StopAfterParser;
    options.printAst = true;
    options.printInput = true;

    grammar::ast::Prog ast1;
    try {
        ast1 = compiler(input, options).ast; 
        if (compiler == grammar::compiler::compileFromFile) {
            input = grammar::compiler::getFileContent(input);
        }
    } catch (const grammar::parser::SyntaxError &e) {   
        std::ostringstream temp;
        temp << ast1;
        std::string ast1_string = temp.str();
        BOOST_CHECK_MESSAGE(testing_outcome == TestingOutcome::PARSE_FAILED, "\nAST:\n" << ast1_string << "\n" <<  e.what() << "\n");
        return;
    }
    std::ostringstream temp1;
    temp1 << ast1;
    std::string ast1_string = temp1.str();

    grammar::ast::Prog ast2;
    try {
        ast2 = grammar::compiler::compileFromString(ast1_string, options).ast; 
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
            BOOST_CHECK_MESSAGE(equal_ignore_whitespaces(input, ast1_string), "\nParse1 output was not consistent with input, when it was expected\n");
            BOOST_CHECK_MESSAGE(equal_ignore_whitespaces(ast1_string, ast2_string), "\nParse2 output was not consistent with Parse1 output, when it was expected\n");
            break;
        case TestingOutcome::PARSE1_INCONSISTENT:
            BOOST_CHECK_MESSAGE(!equal_ignore_whitespaces(input, ast1_string), "\nAST 1 output was consistent with the input, when it was expected not to\n");
            break;
        case TestingOutcome::PARSE2_INCONSISTENT:
            BOOST_CHECK_MESSAGE(!equal_ignore_whitespaces(ast1_string, ast2_string), "\nAST 2 output was consistent with the AST 1 output, when it was expected not to\n");   
            break;
        case TestingOutcome::PARSE_FAILED:
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




