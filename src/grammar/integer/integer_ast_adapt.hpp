#ifndef INTEGER_AST_ADAPT_HPP
#define INTEGER_AST_ADAPT_HPP

#include <boost/fusion/include/adapt_struct.hpp>
#include "integer_ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(grammar::integer_ast::Integer, num); 

#endif 

