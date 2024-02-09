#ifndef SUM_DEF_HPP
#define SUM_DEF_HPP

#include <boost/spirit/home/x3.hpp>

#include "ast/sum_ast.hpp"
#include "ast/sum_ast_adapt.hpp"
#include "sum.hpp"

#include "integer.hpp"
#include "ast/integer_ast.hpp"

namespace grammar 
{

    // our integer parser def 
    namespace parser 
    {
        namespace x3 = boost::spirit::x3; 
        namespace ascii = x3::ascii; 

        

        x3::rule<class sum, sum_ast::sum> const sum = "sum"; 

        auto const sum_def = 
            integer_ast::integer
            >> '+'
            >> integer_ast::integer
            ;

        BOOST_SPIRIT_DEFINE(sum); 

    } // namespace parser 


    parser::sum_type sum() 
    {
        return parser::sum;
    }
} // namespace grammar

#endif