#ifndef MGRAMMAR_AST_HPP
#define MGRAMMAR_AST_HPP 

#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3.hpp>
#include <vector>

class FuncSymbol;
class VarSymbol;
class Symbol;
class SymbolTable;

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
        struct BinopExps;
        struct Block;
        struct ExpressionPar;
        struct FunctionCall;
        struct VarDeclAssign;
        struct VarDeclStatement;
        struct ConditionalStatement;


        struct Id : LocationInfo {
            std::string id;
            // std::variant<std::monostate, VarSymbol*, FuncSymbol*> sym = std::monostate{}; // monostate tells us this can be empty since we can't use nullptr
            Symbol *sym = nullptr;
            SymbolTable *scope = nullptr;
            
        public:
            friend std::ostream& operator<<(std::ostream& os, const Id &exp);
        };

        struct VarExpression : LocationInfo {
            Id id;
            
        public:
            friend std::ostream& operator<<(std::ostream& os, const VarExpression &exp);
        };

        struct PrimitiveType : LocationInfo {
            std::string type; // "int" or "bool"
        public:
            friend std::ostream& operator<<(std::ostream& os, const PrimitiveType &exp); 
        };

        struct Expression : public x3::variant<int, x3::forward_ast<BinopExps>, bool, VarExpression, x3::forward_ast<ExpressionPar>, x3::forward_ast<FunctionCall>>, LocationInfo {
            using base_type::base_type;   
            using base_type::operator=;
        public:
            Expression(const Expression& other) : base_type(other), LocationInfo(other) {}
               
            Expression& operator=(const Expression& other) {
                base_type::operator=(other);
                LocationInfo::operator=(other);
                return *this;
            }
           
            friend std::ostream& operator<<(std::ostream& os, const Expression &exp);
        };

        struct ExpressionPar : LocationInfo {
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ExpressionPar &exp);
        };

        struct Rhs : LocationInfo {
            std::string op;
            Expression exp;
        public: 
            friend std::ostream& operator<<(std::ostream& os, const Rhs &exp);
        };

        struct BinopExps : LocationInfo { 
            Expression lhs;
            std::vector<Rhs> rhss;
            SymbolTable *scope = nullptr;
        public: 
            friend std::ostream& operator<<(std::ostream& os, const BinopExps &exp);
        };

        struct PrintStatement : LocationInfo {
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const PrintStatement &exp);
        };

        struct ReturnStatement : LocationInfo {
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ReturnStatement &exp);
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

        struct Statement : public x3::variant<VarAssign, x3::forward_ast<VarDeclAssign>, x3::forward_ast<VarDeclStatement>, WhileStatement,StatementExpression, PrintStatement, ReturnStatement, x3::forward_ast<ConditionalStatement>>, LocationInfo {
            using base_type::base_type;  
            using base_type::operator=;
        public:
            Statement(const Statement& other) : base_type(other), LocationInfo(other) {}
               
            Statement& operator=(const Statement& other) {
                base_type::operator=(other);
                LocationInfo::operator=(other);
                return *this;
            }
            friend std::ostream& operator<<(std::ostream& os, const Statement &exp);
        };

        struct BlockLine : public x3::variant<x3::forward_ast<Decl> , Statement>, LocationInfo  {
            using base_type::base_type;   
            using base_type::operator=;
        public:
            BlockLine(const BlockLine& other) : base_type(other), LocationInfo(other) {}
               
            BlockLine& operator=(const BlockLine& other) {
                base_type::operator=(other);
                LocationInfo::operator=(other);
                return *this;
            }

            friend std::ostream& operator<<(std::ostream& os, const BlockLine &exp);
        };

        struct Block : LocationInfo {
            std::vector<BlockLine> block_line;
        public:
            friend std::ostream& operator<<(std::ostream& os, const Block &exp);
        };
        
        struct IfStatement : public LocationInfo {
            Expression exp;
            Block block;
        public:
            friend std::ostream& operator<<(std::ostream& os, const IfStatement &exp);
        };

        struct ElseStatement : public LocationInfo {
            Block block;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ElseStatement &exp);
        };

        struct ConditionalStatement : public LocationInfo {
            IfStatement ifStatement;
            std::vector<IfStatement> elseIfs; 
            // x3::variant<ElseStatement, x3::unused_type> conditionalElse;
            boost::optional<ElseStatement> conditionalElse;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ConditionalStatement &exp);
        };

        struct Type : public x3::variant<PrimitiveType /*, ArrayType*/>, LocationInfo {
            using base_type::base_type;   
            using base_type::operator=;
        public:
            Type(const Type& other) : base_type(other), LocationInfo(other) {}
               
            Type& operator=(const Type& other) {
                base_type::operator=(other);
                LocationInfo::operator=(other);
                return *this;
            }
            friend std::ostream& operator<<(std::ostream& os, const Type &exp);
        };

        struct VarDecl : public LocationInfo {
            Type type;
            Id id;
            VarSymbol *sym = nullptr;
        public:
            friend std::ostream& operator<<(std::ostream& os, const VarDecl &exp);
        };

        struct VarDeclAssign : public LocationInfo {
            VarDecl decl;
            Expression exp;
            friend std::ostream& operator<<(std::ostream& os, const VarDeclAssign &exp);
        };

        struct VarDeclStatement : public x3::variant<VarDecl> , LocationInfo {
            using base_type::base_type;  
            using base_type::operator=;

            VarDeclStatement(const VarDeclStatement& other) : base_type(other), LocationInfo(other) {}
               
            VarDeclStatement& operator=(const VarDeclStatement& other) {
                base_type::operator=(other);
                LocationInfo::operator=(other);
                return *this;
            }
            friend std::ostream& operator<<(std::ostream& os, const VarDeclStatement &exp);
        };

        // this is a variant because later it needs to be extended to include optional parameters
        struct Parameter : public x3::variant<VarDecl>, LocationInfo {
            using base_type::base_type;  
            using base_type::operator=;

            Parameter(const Parameter& other) : base_type(other), LocationInfo(other) {}
               
            Parameter& operator=(const Parameter& other) {
                base_type::operator=(other);
                LocationInfo::operator=(other);
                return *this;
            }
            friend std::ostream& operator<<(std::ostream& os, const Parameter &exp);
        };        

        struct ParameterList : LocationInfo { 
            std::vector<Parameter> parameter; 
        public:
            friend std::ostream& operator<<(std::ostream& os, const ParameterList &exp);
        };
         

        struct ArgumentList : LocationInfo {
            std::vector<Expression> arguments;
            friend std::ostream& operator<<(std::ostream& os, const ArgumentList &argument);
        };

        struct FunctionCall : LocationInfo {
            Id id; 
            ArgumentList argument_list;
            friend std::ostream& operator<<(std::ostream& os, const FunctionCall &funcCall);
        };

        struct FuncDecl : public LocationInfo { 
            FuncDecl();
            ~FuncDecl();

            Type type;  
            Id id;  
            ParameterList parameter_list;  
            Block block; 
            FuncSymbol *sym = nullptr;
        public:
            friend std::ostream& operator<<(std::ostream& os, const FuncDecl &exp);
        };
        
        struct ArrayType : LocationInfo {
            Type type;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ArrayType &exp);
        };

        struct Decl : public x3::variant<VarDeclAssign, VarDeclStatement, FuncDecl/*, ClassDecl*/>, LocationInfo { 
            using base_type::base_type;  
            using base_type::operator=;
        public:
            Decl(const Decl& other) : base_type(other), LocationInfo(other) {}
               
            Decl& operator=(const Decl& other) {
                base_type::operator=(other);
                LocationInfo::operator=(other);
                return *this;
            }

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
    BinopExps,
    (Expression, lhs),
    (std::vector<Rhs>, rhss)
)

BOOST_FUSION_ADAPT_STRUCT(
    Rhs,
    (std::string, op),
    (Expression, exp)
)


BOOST_FUSION_ADAPT_STRUCT(
    PrintStatement,
    (Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    VarExpression,
    (Id, id)
)

BOOST_FUSION_ADAPT_STRUCT(
    IfStatement,
    (Expression, exp),
    (Block, block)
)


BOOST_FUSION_ADAPT_STRUCT(
    ElseStatement,
    (Block, block)
)


BOOST_FUSION_ADAPT_STRUCT(
    ConditionalStatement,
    (IfStatement, ifStatement),
    (std::vector<IfStatement>, elseIfs), 
    (boost::optional<ElseStatement>, conditionalElse)
    // (x3::variant<ElseStatement, x3::unused_type>, conditionalElse)
)


BOOST_FUSION_ADAPT_STRUCT(
    ReturnStatement,
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
)

BOOST_FUSION_ADAPT_STRUCT(
    VarDeclAssign,
    (VarDecl, decl)
    (Expression, exp)
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
