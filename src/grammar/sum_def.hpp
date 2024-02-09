#ifndef SUM_DEF_HPP
#define SUM_DEF_HPP

#include <boost/spirit/home/x3.hpp>

#include "ast/sum_ast.hpp"
#include "ast/sum_ast_adapt.hpp"
#include "sum.hpp"
#include "integer_def.hpp"

namespace grammar 
{

    // our integer parser def 
    namespace parser 
    {
        namespace x3 = boost::spirit::x3; 
        namespace ascii = x3::ascii; 

        

        x3::rule<class sum, sum_ast::sum> const sum = "sum"; 

        auto const sum_def = 
            parser::integer
            >> '+'
            >> parser::integer
            ;
            

        BOOST_SPIRIT_DEFINE(sum); 

    } // namespace parser 


    parser::sum_type sum() 
    {
        return parser::sum;
    }
} // namespace grammar

#endif