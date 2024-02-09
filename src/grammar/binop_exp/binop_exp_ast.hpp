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

        enum binop_op {
            op_eq,
            op_neq,
            op_leq,
            op_geq,
            op_add,
            op_minus,
            op_times,
            op_div,
            op_mod,
            op_lt,
            op_gt,
            op_and,
            op_or
        };

        struct binop_exp
        { 
            integer_ast::integer lhs;
            binop_op op;
            integer_ast::integer rhs;
        };

        using boost::fusion::operator<<;

    } 
}
#endif