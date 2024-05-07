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

public:
    ast::Prog ast;
    std::unique_ptr<SymbolTable> global_scope;
    IR ir;
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
    std::optional<StopAfter> stop_after;
    bool print_ast = false ;
    bool print_input = false;
    bool print_code_generation = false;
    bool print_register_allocation = false;
};

std::string get_file_content(std::string_view input);
std::unique_ptr<CompilerReturnObj> compile_from_file(std::string_view input, const CompilerOptions &options);
std::unique_ptr<CompilerReturnObj> compile_from_string(std::string_view input, const CompilerOptions &options);

}

#endif
