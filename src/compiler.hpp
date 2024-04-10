#ifndef MGRAMMAR_COMPILER_HPP
#define MGRAMMAR_COMPILER_HPP 

#include "semantics/symbol_table.hpp"
#include <optional>
#include <memory>
#include <string_view>

// ALl 3 of these include IR.hpp (so we include it 3 times here)
#include "IR/register_allocation.hpp"
#include "IR/code_generation.hpp"
#include "IR/emit.hpp"


namespace grammar::compiler {

struct CompilerReturnObj {
private:
    std::unique_ptr<SymbolTable> globalScope;

public:
    ast::Prog ast; 
    // Other options not implemented 
    IR ir;
    // Essembly

    void setGlobalScope(std::unique_ptr<SymbolTable> globalScope) {
        this->globalScope = std::move(globalScope);
    }
};

enum StopAfter {
    StopAfterParser,
    StopAfterTypeCheck,
    StopAfterSymbolCollection,
    StopAfterCodeGen,
    StopAfterRegAlloc
};

class CompilerOptions {
public:
    std::optional<StopAfter> stopAfter;
    bool printAst = false ;
    bool printInput = false;
    bool printCodeGeneration = false;
    bool printRegisterAllocation = false;
};

std::string getFileContent(std::string_view input);
std::unique_ptr<CompilerReturnObj> compileFromFile(std::string_view input, const CompilerOptions &options);
std::unique_ptr<CompilerReturnObj> compileFromString(std::string_view input, const CompilerOptions &options);

}

#endif
