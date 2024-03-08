#include <iostream>
#include <stdexcept>
#include <string_view>
#include <fstream>
#include <sstream>
#include <memory>
#include "compiler.hpp"
#include "ast.hpp"
#include "parser/parser.hpp"
#include "semantics/symbol_collection.hpp"
#include "type_checking/typeChecking.hpp"


namespace grammar::compiler {
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
    std::unique_ptr<CompilerReturnObj> compileFromFile(std::string_view input, const CompilerOptions &options) {
        std::string content = getFileContent(input);
        auto obj = compileFromString(std::string_view(content), options);

        return obj;
    }

    // compile from a string
    std::unique_ptr<CompilerReturnObj> compileFromString(std::string_view input, const CompilerOptions &options) {
        if (options.printInput) {
            std::cout << "Input to be parsed: \n" << input;
        }   
 
        auto obj = std::make_unique<CompilerReturnObj>();
        obj->ast = parser::parse(input);
        
        // print ast tree if option is enabled
        if (options.printAst) {
            std::cout << "AST:\n" << obj->ast;
        }

        if (options.stopAfter == StopAfterParser ) {
            return obj;
        }

        obj->ast = symbol_collection(obj->ast);

        if (options.stopAfter == StopAfterSymbolCollection ) {
             return obj;
        }


        obj->ast = typeChecker(obj->ast);

        if (options.stopAfter == StopAfterTypeCheck ) {
             return obj;
        }

        return obj;
    }

} // namespace grammar::compiler
