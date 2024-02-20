#include <string_view>


struct CompilerOptions {
    std::string_view input; 

};

void compile(CompilerOptions options);
