#ifndef MGRAMMAR_GRAMMAR_HPP
#define MGRAMMAR_GRAMMAR_HPP


#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace grammar 
{

    namespace parser 
    {
        namespace x3 = boost::spirit::x3;       
        using BinopExp_type = x3::rule<class binop_exp, ast::BinopExp>; 
        BOOST_SPIRIT_DECLARE(BinopExp_type); 

    }   

    parser::BinopExp_type BinopExp();  
} 


#endif