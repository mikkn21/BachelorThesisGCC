#ifndef BINOP_AST_ADAPT_HPP
#define BINOP_AST_ADAPT_HPP

#include <boost/fusion/include/adapt_struct.hpp>
#include "binop_exp_ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(grammar::binop_exp_ast::binop_exp, lhs, op, rhs); 

#endif 

