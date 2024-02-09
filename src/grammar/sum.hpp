#ifndef MGRAMMAR_SUM_HPP
#define MGRAMMAR_SUM_HPP


#include <boost/spirit/home/x3.hpp>

#include "ast/sum_ast.hpp"

namespace grammar 
{

    // This is our sum declaration
    namespace parser 
    {
        namespace x3 = boost::spirit::x3;       
        using sum_type = x3::rule<class sum, sum_ast::sum>; 
        BOOST_SPIRIT_DECLARE(sum_type); 
    }   

    parser::sum_type sum();  
} 

#endif