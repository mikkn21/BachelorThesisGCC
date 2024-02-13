#ifndef MGRAMMAR_BINOP_EXP_AST_HPP
#define MGRAMMAR_BINOP_EXP_AST_HPP 

#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string> 
#include "../integer/integer_ast.hpp"

namespace grammar 
{ 
    namespace binop_exp_ast 
    {
        // Our integer struct 

        struct BinopExp
        { 
            integer_ast::Integer lhs;
            std::string op;
            integer_ast::Integer rhs;
        };

        using boost::fusion::operator<<;

    } 
}
#endif