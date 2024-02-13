#ifndef MGRAMMAR_INTEGER_HPP
#define MGRAMMAR_INTEGER_HPP


#include <boost/spirit/home/x3.hpp>

#include "integer_ast.hpp"

namespace grammar 
{

    // This is our integer declaration
    namespace parser 
    {
        namespace x3 = boost::spirit::x3;       
        using integer_type = x3::rule<class integer, integer_ast::Integer>; 
        BOOST_SPIRIT_DECLARE(integer_type); 
    }   

} 

#endif