#ifndef MGRAMMAR_COMPILER_HPP
#define MGRAMMAR_COMPILER_HPP 

#include "ast.hpp"
#include "semantics/symbol_table.hpp"
#include <optional>
#include <memory>
#include <string_view>

namespace grammar::compiler {

using namespace std;

struct CompilerReturnObj {
private:
     unique_ptr<SymbolTable> globalScope;

public:
    ast::Prog ast; 
    // Other options not implemented 
    // IR 
    // Essembly

    void setGlobalScope(unique_ptr<SymbolTable> globalScope) {
        this->globalScope = std::move(globalScope);
    }
};

enum StopAfter {
    StopAfterParser,
    StopAfterTypeCheck,
    StopAfterSymbolCollection
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
