#include "compiler.hpp"
#include "parser/parser.hpp"


void compile(CompilerOptions options) {


    bool res = grammar::parser::parse(options.input);

}