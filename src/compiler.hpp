#ifndef MGRAMMAR_COMPILER_HPP
#define MGRAMMAR_COMPILER_HPP 

#include "ast.hpp"
#include <optional>
#include <string_view>

namespace grammar::compiler {


struct CompilerReturnObj {
    ast::Prog ast; 
    // Other options not implemented 
    // IR 
    // Essembly
};

enum StopAfter {
    StopAfterParser
};

class CompilerOptions {
public:
    std::optional<StopAfter> stopAfter;
    bool printAst = false ;
    bool printInput = false;

};


std::string getFileContent(std::string_view input);
std::unique_ptr<CompilerReturnObj> compileFromFile(std::string_view input, const CompilerOptions &options);
std::unique_ptr<CompilerReturnObj> compileFromString(std::string_view input, const CompilerOptions &options);

}

#endif
