#ifndef INTEGER_DEF_HPP
#define INTEGER_DEF_HPP

#include <boost/spirit/home/x3.hpp>

#include "integer_ast.hpp"
#include "integer_ast_adapt.hpp"
#include "integer.hpp"

namespace grammar 
{

    // our integer parser def 
    namespace parser 
    {
        namespace x3 = boost::spirit::x3; 
        namespace ascii = x3::ascii; 

        using x3::int_;

        x3::rule<class integer, integer_ast::Integer> const integer = "integer"; 

        auto const integer_def = int_;

        BOOST_SPIRIT_DEFINE(integer); 

    } // namespace parser 

} // namespace grammar

#endif