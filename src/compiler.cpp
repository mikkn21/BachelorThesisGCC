#include "compiler.hpp"
#include "parser/parser.hpp"


void compile(CompilerOptions options) {


    grammar::ast::Prog res = grammar::parser::parse(options.input);

}