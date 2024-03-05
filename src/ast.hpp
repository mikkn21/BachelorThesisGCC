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

        struct LocationInfo {
        	std::size_t line, column;
        public:
            friend std::ostream& operator<<(std::ostream& os, const LocationInfo &location_info);
        };

        struct Decl;
        struct BinopExp;
        struct Block;
        struct ExpressionPar;


        struct Id : LocationInfo {
            std::string id;
        public:
            friend std::ostream& operator<<(std::ostream& os, const Id &exp);
        };

        struct PrimitiveType : LocationInfo {
            std::string type; // "int" or "bool"
        public:
            friend std::ostream& operator<<(std::ostream& os, const PrimitiveType &exp); 
        };

        struct Expression : x3::variant<int, x3::forward_ast<BinopExp>, Id, bool, x3::forward_ast<ExpressionPar>>, LocationInfo {
            using base_type::base_type;   
            using base_type::operator=;
        public:
            friend std::ostream& operator<<(std::ostream& os, const Expression &exp);
        };

        struct ExpressionPar : LocationInfo {
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ExpressionPar &exp);
        };

        struct BinopExp : LocationInfo { 
            Expression lhs;
            std::string op;
            Expression rhs;
        public: 
            friend std::ostream& operator<<(std::ostream& os, const BinopExp &exp);
        };

        struct PrintStatement : LocationInfo {
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const PrintStatement &exp);
        };

        struct VarAssign : LocationInfo {
            Id id; 
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const VarAssign &exp);
        };

        struct WhileStatement : LocationInfo {
            Expression exp; 
            x3::forward_ast<Block> block;
        public:
            friend std::ostream& operator<<(std::ostream& os, const WhileStatement &exp);
        };

        struct StatementExpression : LocationInfo {
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const StatementExpression &exp);
        };

        struct Statement : x3::variant<VarAssign, WhileStatement,StatementExpression, PrintStatement>, LocationInfo {
            using base_type::base_type;  
            using base_type::operator=;
        public:
            friend std::ostream& operator<<(std::ostream& os, const Statement &exp);
        };


        struct BlockLine : x3::variant<x3::forward_ast<Decl> , Statement>, LocationInfo  {
            using base_type::base_type;   
            using base_type::operator=;
        public:
            friend std::ostream& operator<<(std::ostream& os, const BlockLine &exp);
        };

        struct Block : LocationInfo {
            std::vector<BlockLine> block_line;
        public:
            friend std::ostream& operator<<(std::ostream& os, const Block &exp);
        };

        struct Type : x3::variant<PrimitiveType /*, ArrayType*/>, LocationInfo {
            using base_type::base_type;   
            using base_type::operator=;
        public:
            friend std::ostream& operator<<(std::ostream& os, const Type &exp);
        };

        struct VarDecl : LocationInfo {
            Type type;
            Id id;
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const VarDecl &exp);
        };

        struct Parameter : LocationInfo {
            Type type;
            Id id; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const Parameter &exp);
        };        

        struct ParameterList : LocationInfo { 
            std::vector<Parameter> parameter; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const ParameterList &exp);
        };
         

        struct ArgumentList : LocationInfo {
            std::vector<Expression> arguments;
        };

        struct FunctionCall : LocationInfo {
            Id id; 
            ArgumentList argument_list;
        };

        struct FuncDecl : LocationInfo { 
            Type type;  
            Id id;  
            ParameterList parameter_list;  
            Block block; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const FuncDecl &exp);
        };
        
        struct ArrayType : LocationInfo {
            Type type;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ArrayType &exp);
        };


        struct Decl : x3::variant<VarDecl, FuncDecl/*, ClassDecl*/>, LocationInfo { 
            using base_type::base_type;  
            using base_type::operator=;
        public:
            friend std::ostream& operator<<(std::ostream& os, const Decl &exp);
        };        

        struct Prog : LocationInfo {
            std::vector<Decl> decls; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const Prog& exp);
        };

    } // namespace ast
} // namespace grammar 


using namespace grammar::ast;


BOOST_FUSION_ADAPT_STRUCT(
    BinopExp,
    (Expression, lhs)
    (std::string, op)
    (Expression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    PrintStatement,
    (Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    StatementExpression, 
    (Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    Id,
    (std::string, id)
)

BOOST_FUSION_ADAPT_STRUCT(
    ExpressionPar,
    (Expression, exp)
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
    (std::vector<Decl>, decls)
)


#endif
