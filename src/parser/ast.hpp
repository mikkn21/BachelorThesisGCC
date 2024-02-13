#ifndef MGRAMMAR_AST_HPP
#define MGRAMMAR_AST_HPP 

#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/adapt_struct.hpp>


namespace grammar 
{ 
    namespace ast
    {     
        struct PrimitiveType {
            std::string type;
        };

        struct Id {
            std::string id;
        };

        struct Type {
          PrimitiveType primitive_type;  
        };

        struct VarDecl {
          Type type;
          Id id;
          int exp;
        }; 
          
        struct Decl {
            VarDecl var_decl; 
        };

        struct Prog {
            Decl decl; 
        };


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

using namespace grammar::ast;

BOOST_FUSION_ADAPT_STRUCT(BinopExp,
                         (int, lhs),
                         (std::string, op),
                         (int, rhs)
                          );

BOOST_FUSION_ADAPT_STRUCT(Prog, (Decl, decl) );
BOOST_FUSION_ADAPT_STRUCT(Decl, (VarDecl, var_decl) );
BOOST_FUSION_ADAPT_STRUCT(VarDecl, 
                         (Type, type),
                         (Id, id),
                         (int, exp),
                         );

BOOST_FUSION_ADAPT_STRUCT(Type, (PrimitiveType, primitive_type) );
BOOST_FUSION_ADAPT_STRUCT(Id, (std::string, id) );
BOOST_FUSION_ADAPT_STRUCT(PrimitiveType, (std::string, type) );



#endif