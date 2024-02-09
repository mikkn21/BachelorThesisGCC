#ifndef SUM_AST_ADAPT_HPP
#define SUM_AST_ADAPT_HPP

#include <boost/fusion/include/adapt_struct.hpp>
#include "sum_ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(grammar::sum_ast::sum, num1, num2);

#endif