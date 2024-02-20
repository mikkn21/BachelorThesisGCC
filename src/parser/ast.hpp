#ifndef MGRAMMAR_AST_HPP
#define MGRAMMAR_AST_HPP 

#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <vector>

namespace x3 = boost::spirit::x3;

namespace grammar 
{ 
    namespace ast
    {   

        struct Decl;


        struct BinopExp { 
            int lhs;
            std::string op;
            int rhs;
        public: 
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::BinopExp &exp);
        };

        struct Id {
            std::string id;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::Id &exp);
        };

        struct PrimitiveType {
            std::string type; // "int" or "bool"
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::PrimitiveType &exp); 
        };

        struct Expression : x3::variant<int, BinopExp, bool> {
            using base_type::base_type;   
            using base_type::operator=;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::Expression &exp);
        };

        struct BlockLine : x3::variant<x3::forward_ast<Decl> /*, statement*/>  {
            using base_type::base_type;   
            using base_type::operator=;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::BlockLine &exp);
        };

        struct Block {
            std::vector<BlockLine> block_line;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::Block &exp);
        };

        struct Type {
            PrimitiveType primitive_type;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::Type &exp);
        };

        struct VarDecl {
            Type type;
            Id id;
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::VarDecl &exp);
        };

        struct Parameter {
            Type type;
            Id id; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::Parameter &exp);
        };        

        struct ParameterList { 
            std::vector<Parameter> parameter; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::ParameterList &exp);
        };
         

        struct ArgumentList {
            std::vector<Expression> arguments;
        };

        struct FunctionCall {
            Id id; 
            ArgumentList argument_list;
        };

        struct FuncDecl { 
            Type type;  
            Id id;  
            ParameterList parameter_list;  
            Block block; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::FuncDecl &exp);
        };
        
        struct ArrayType {
            Type type;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::ArrayType &exp);
        };

        struct VarAssign {
            Id id; 
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::VarAssign &exp);
        };

        struct WhileStatement {
            Expression exp; 
            Block block;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::WhileStatement &exp);
        };

        struct Decl : x3::variant<VarDecl, FuncDecl/*, ClassDecl*/> { 
            using base_type::base_type;  
            using base_type::operator=;
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::Decl &exp);
        };        

        struct Prog {
            Decl decl; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const grammar::ast::Prog& exp);
        };

        //using boost::fusion::operator<<;

    } // namespace ast
} // namespace grammar 


using namespace grammar::ast;


BOOST_FUSION_ADAPT_STRUCT(
    BinopExp,
    (int, lhs)
    (std::string, op)
    (int, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    Id,
    (std::string, id)
)

BOOST_FUSION_ADAPT_STRUCT(
    ArgumentList, 
    (std::vector<Expression>, arguments)
)
 

BOOST_FUSION_ADAPT_STRUCT(
    PrimitiveType,
    (std::string, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    FunctionCall, 
    (Id, id), 
    (ArgumentList, argument_list)
)

BOOST_FUSION_ADAPT_STRUCT(
    Block,
    (std::vector<BlockLine>, block_line)
)

BOOST_FUSION_ADAPT_STRUCT(
    Type,
    (PrimitiveType, primitive_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    VarDecl,
    (Type, type)
    (Id, id)
    (Expression, exp) // Assuming exp is an int for simplicity
)

BOOST_FUSION_ADAPT_STRUCT(
    Parameter,
    (Type, type)
    (Id, id)
)

BOOST_FUSION_ADAPT_STRUCT(
    ParameterList,
    (std::vector<Parameter>, parameter)
)

BOOST_FUSION_ADAPT_STRUCT(
    FuncDecl,
    (Type, type)
    (Id, id)
    (ParameterList, parameter_list)
    (Block, block)
)

BOOST_FUSION_ADAPT_STRUCT(
    ArrayType,
    (Type, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    VarAssign,
    (Id, id),
    (Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    WhileStatement,
    (Expression, exp),
    (Block, block)
)

BOOST_FUSION_ADAPT_STRUCT(
    Prog,
    (Decl, decl)
)


#endif