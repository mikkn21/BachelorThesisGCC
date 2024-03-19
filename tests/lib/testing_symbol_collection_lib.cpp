#include "testing_symbol_collection_lib.hpp"
#include "../../src/ast.hpp"
#include "../../src/compiler.hpp"
#include "../../src/semantics/semantics_error.hpp"
#include <string>
#include "testing_symbol_collection_lib.hpp"
#include "../../src/error/base_error.hpp"


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
void testSymbolCollection(std::string input, TestingOutcome testing_outcome, Compiler compiler) {
    std::cout << "\n----------------------" << std::endl;
    grammar::compiler::CompilerOptions options = grammar::compiler::CompilerOptions();
    options.stopAfter = grammar::compiler::StopAfterSymbolCollection;
    options.printAst = false;
    options.printInput = true;

    grammar::ast::Prog ast1;
    try {
        ast1 = compiler(input, options)->ast; 
    } catch (const SemanticsError &e) {   
        BOOST_CHECK_MESSAGE(testing_outcome == TestingOutcome::FAILED, "\n"  <<  e.what() << "\n");
        return;
    } catch (const CompilerError &e) {   
        BOOST_CHECK_MESSAGE(false, "\n--- An unknown error derived from Baseerror was encountered in the test.\n" << e.what());
        return;
    } catch (exception &e) {
        BOOST_CHECK_MESSAGE(false, "\n--- An unknown error type was encountered in the test.\n" << e.what());
        return;
    } catch (...) {
        BOOST_CHECK_MESSAGE(false, "\n--- Caught exception/error we did not recognize.\n");
        return;
    }
        
    BOOST_CHECK_MESSAGE(testing_outcome == TestingOutcome::SUCCESS, "\n" << "succeded while it expected to fail " << "\n");
    return; 
}

void testSymbolCollectionFile(std::string input, TestingOutcome testing_outcome) {
    testSymbolCollection(input, testing_outcome, grammar::compiler::compileFromFile);
}

void testSymbolCollectionString(std::string input, TestingOutcome testing_outcome) {
    testSymbolCollection(input, testing_outcome, grammar::compiler::compileFromString);
}




