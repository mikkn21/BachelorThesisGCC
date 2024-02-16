#ifndef MGRAMMAR_AST_HPP
#define MGRAMMAR_AST_HPP 

#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <vector>


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
        //Expression exp;
        };

        /* struct Type : boost::variant<PrimitiveType, ArrayType> { */
        /*     using base_type::base_type;  */
        /* };  */

       struct Decl {
            VarDecl var_decl; 
        };

        struct BlockLine {
            Decl decl; 
        };

        struct Block {
            BlockLine block_line;

        };


        struct VarAssign {
            Id id; 
            //Expression exp;

        };

        struct WhileStatement {
            //Expression exp; 
            Block block;
        };


 
        
        /* struct Decl : boost::variant<VarDecl, FuncDecl, ClassDecl> { */
        /*     using base_type::base_type;  */
        /**/
        /* }; */


        struct Parameter {
            Type type;
            Id id; 
        };

        /* struct ParameterList { */
        /*     std::vector<Parameter> parameter; */
        /* }; */

        struct ArrayType {
            Type type;
        };     



        /* struct FuncDecl { */
        /*     Type type;  */
        /*     Id id;  */
        /*     ParameterList parameter_list;  */
        /*     Block block; */
        /**/
        /* }; */
        /**/


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

/* BOOST_FUSION_ADAPT_STRUCT(FuncDecl, (Type, type), (Id, id), (ParameterList, parameter_list), (Block, block) ); */
BOOST_FUSION_ADAPT_STRUCT(VarDecl, 
                         (Type, type),
                         (Id, id),
                         (int, exp),
                         );
BOOST_FUSION_ADAPT_STRUCT(Id, (std::string, id) );
BOOST_FUSION_ADAPT_STRUCT(Type, (PrimitiveType, primitive_type) );
BOOST_FUSION_ADAPT_STRUCT(PrimitiveType, (std::string, type) );

BOOST_FUSION_ADAPT_STRUCT(ArrayType, (Type, type), (Id, id) );

/* BOOST_FUSION_ADAPT_STRUCT(ParameterList, (std::vector<Parameter> paramete) ); */

BOOST_FUSION_ADAPT_STRUCT(Parameter, (Type, type), (Id, id) );

BOOST_FUSION_ADAPT_STRUCT(Block, (BlockLine, block_line));
BOOST_FUSION_ADAPT_STRUCT(BlockLine, (Decl, decl));

BOOST_FUSION_ADAPT_STRUCT(VarAssign, (Id, id));
BOOST_FUSION_ADAPT_STRUCT(WhileStatement, (Block, block));




#endif
