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
    bool printAst;
    bool printInput;



    CompilerOptions();
};

CompilerReturnObj compileFromFile(std::string_view input, const CompilerOptions &options);
CompilerReturnObj compileFromString(std::string_view input, const CompilerOptions &options);

}

#endif
