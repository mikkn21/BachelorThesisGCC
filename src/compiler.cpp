#include <iostream>
#include <stdexcept>
#include <string_view>
#include <fstream>
#include <sstream>

#include "compiler.hpp"
#include "ast.hpp"
#include "parser/parser.hpp"
#include "semantics/symbol_collection.hpp"


// NOTE: Add default values for ALL the compiler options
namespace grammar::compiler {
    CompilerOptions::CompilerOptions() {
        stopAfter = std::nullopt;
        printAst = false;
        printInput = false;
    }

    std::string getFileContent(std::string_view input) {
        if(input.empty()) {
            std::cerr << "Error: filename is empty!" << std::endl;
            throw std::invalid_argument("Filename is empty"); 
        }

        std::ifstream file(input.data());
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << input << std::endl;
            throw std::invalid_argument("Could not open file correctly, ABORT! "); 
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();
        return content;
    }

    // This is compiling from a filename
    CompilerReturnObj compileFromFile(std::string_view input, const CompilerOptions &options) {
        std::string content = getFileContent(input);
        CompilerReturnObj obj = compileFromString(std::string_view(content), options);

        if(options.stopAfter == StopAfterParser ) {
            return obj;
        } // NOTE: add the other stopAfter options at some point

        return obj;
    }

    // compile from a string
    CompilerReturnObj compileFromString(std::string_view input, const CompilerOptions &options) {
        if (options.printInput) {
            std::cout << "Input to be parsed: \n" << input;
        }   

        CompilerReturnObj obj; 
        obj.ast = parser::parse(input);
      
        // print ast tree if option is enabled
        if (options.printAst) {
            std::cout << "AST:\n" << obj.ast;
        }

        if (options.stopAfter == StopAfterParser ) {
            return obj;
        } // NOTE: add the other stopAfter options at some point

        return obj;
    }

} // namespace grammar::compiler
