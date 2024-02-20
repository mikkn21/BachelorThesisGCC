#include <iostream>
#include <string_view>
#include <fstream>
#include <sstream>

#include "compiler.hpp"
#include "parser/parser.hpp"

namespace grammar::compiler {
CompilerOptions::CompilerOptions() {
    stopAfter = std::nullopt;
    printAst = false;
}

// This is compiling from a filename
void compile(std::string_view input, const CompilerOptions &options) {
    if(input.empty()) {
        std::cerr << "Error: filename is empty!" << std::endl;
        return;
    }

    std::ifstream file(input.data());
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    ast::Prog res = parser::parse(std::string_view(content));
 }

// compile from a string
void compileFromString(std::string_view input, const CompilerOptions &options) {
    if(input.empty()) {
    std::cerr << "Error: input string is empty!" << std::endl;
    return;
    }

    ast::Prog res = parser::parse(input);
 }

} // namespace grammar::compiler