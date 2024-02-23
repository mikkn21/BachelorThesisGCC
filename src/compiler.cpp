#include <iostream>
#include <stdexcept>
#include <string_view>
#include <fstream>
#include <sstream>

#include "compiler.hpp"
#include "ast.hpp"
#include "parser/parser.hpp"
#include "semantics/symbol_collection.hpp"

namespace grammar::compiler {
    CompilerOptions::CompilerOptions() {
        stopAfter = std::nullopt;
        printAst = false;
    }

    // This is compiling from a filename
    CompilerReturnObj compileFromFile(std::string_view input, const CompilerOptions &options) {
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


        CompilerReturnObj obj = compileFromString(std::string_view(content), options);

        if(options.stopAfter == StopAfterParser ) {
            return obj;
        } // NOTE: add the other stopAfter options at some point

        return obj;
    }

    // compile from a string
    CompilerReturnObj compileFromString(std::string_view input, const CompilerOptions &options) {
        if (input.empty()) {
            std::cerr << "Error: input string is empty!" << std::endl;
            throw std::invalid_argument("String is empty");
        }

        if (options.printInput) {
            std::cout << "Input to be parsed: \n" << input << std::endl;
        }   

        CompilerReturnObj obj; 
        obj.ast = parser::parse(input);
        symbol_collection(obj.ast);
        // print ast tree if option is enabled
        if (options.printAst) {
            std::cout << "AST:\n" << obj.ast << std::endl;
        }

        if (options.stopAfter == StopAfterParser ) {
            return obj;
        } // NOTE: add the other stopAfter options at some point

        return obj;
    }

} // namespace grammar::compiler
