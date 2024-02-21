#include <iostream>
#include <stdexcept>
#include <string_view>
#include <fstream>
#include <sstream>

#include "compiler.hpp"
#include "parser/ast.hpp"
#include "parser/parser.hpp"

namespace grammar::compiler {
CompilerOptions::CompilerOptions() {
    stopAfter = std::nullopt;
    printAst = false;
}

// This is compiling from a filename
CompilerReturnObj compile(std::string_view input, const CompilerOptions &options) {
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

    return compileFromString(std::string_view(content), options);

 }

// compile from a string
CompilerReturnObj compileFromString(std::string_view input, const CompilerOptions &options) {
    if(input.empty()) {
    std::cerr << "Error: input string is empty!" << std::endl;
    throw std::invalid_argument("String is empty"); 
    
    }
    CompilerReturnObj compilerObj; 
    compilerObj.ast = parser::parse(input);

    return compilerObj;
 }

} // namespace grammar::compiler
