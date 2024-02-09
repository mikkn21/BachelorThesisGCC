#ifndef MGRAMMAR_SUM_AST_HPP
#define MGRAMMAR_SUM_AST_HPP 

#include <boost/fusion/include/io.hpp>

#include "integer_ast.hpp"

#include <iostream>
#include <string> 


namespace grammar 
{ 
    namespace sum_ast 
    {
        // Our sum struct 

        struct sum 
        { 
            integer_ast::integer num1;
            integer_ast::integer num2; 
        };

        using boost::fusion::operator<<;

    } // namespace sum_ast
}
#endif