#ifndef MGRAMMAR_INTEGER_AST_HPP
#define MGRAMMAR_INTEGER_AST_HPP 

#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string> 


namespace grammar 
{ 
    namespace integer_ast 
    {
        // Our integer struct 

        struct integer 
        { 
            int num; 
        };

        using boost::fusion::operator<<;

    } 
}
#endif