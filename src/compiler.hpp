#ifndef MGRAMMAR_COMPILER_HPP
#define MGRAMMAR_COMPILER_HPP 

#include <optional>
#include <string_view>

namespace grammar::compiler {

enum StopAfter {
    StopAfterParser
};

class CompilerOptions {
public:
    std::optional<StopAfter> stopAfter;
    bool printAst;

    CompilerOptions();
};

void compile(std::string_view input, const CompilerOptions &options);
void compileFromString(std::string_view input, const CompilerOptions &options);

}

#endif
