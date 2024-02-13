#ifndef MGRAMMAR_AST_HPP
#define MGRAMMAR_AST_HPP 

#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/adapt_struct.hpp>


namespace grammar 
{ 
    namespace ast
    {        
        struct BinopExp
        { 
            int lhs;
            std::string op;
            int rhs;
        };

        //using boost::fusion::operator<<;

    } // namespace ast
} // namespace grammar 


// BOOST_FUSION_ADAPT_STRUCT(grammar::ast::BinopExp,
//                          (int, lhs),
//                          (std::string, op),
//                          (int, rhs),
//                           );

BOOST_FUSION_ADAPT_STRUCT(grammar::ast::BinopExp,
                         lhs,
                         op,
                         rhs
                          );


#endif