#ifndef MGRAMMAR_BINOP_EXP_HPP
#define MGRAMMAR_BINOP_EXP_HPP


#include <boost/spirit/home/x3.hpp>

#include "binop_exp_ast.hpp"

namespace grammar 
{

    // This is our integer declaration
    namespace parser 
    {
        namespace x3 = boost::spirit::x3;       
        using binop_exp_type = x3::rule<class binop_exp, binop_exp_ast::binop_exp>; 
        BOOST_SPIRIT_DECLARE(binop_exp_type); 
    }   

    parser::binop_exp_type binop_exp();  
} 

#endif