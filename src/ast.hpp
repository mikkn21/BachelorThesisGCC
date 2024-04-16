#ifndef MGRAMMAR_AST_HPP
#define MGRAMMAR_AST_HPP 


#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>
#include <vector>

class FuncSymbol;
class ClassSymbol;
class VarSymbol;
class Symbol;
class SymbolTable;

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
        struct ArrayType;
        struct ArrayExp;
        struct ArrayIndex;
        struct ObjInst;

        struct Id : LocationInfo {
            std::string id;
            Symbol *sym = nullptr;
            SymbolTable *scope = nullptr;
            
        public:
            friend std::ostream& operator<<(std::ostream& os, const Id &exp);
        };

        struct BreakStatement : public LocationInfo {
        public:
            friend std::ostream& operator<<(std::ostream& os, const BreakStatement &exp);
        };

        struct ContinueStatement : public LocationInfo {
        public:
            friend std::ostream& operator<<(std::ostream& os, const ContinueStatement &exp);
        };

        struct IdAccess : public LocationInfo {
            std::vector<Id> ids;  
        public:
            friend std::ostream& operator<<(std::ostream& os, const IdAccess &exp);
        };

        struct VarExpression : LocationInfo {
            IdAccess idAccess;
        public:
            friend std::ostream& operator<<(std::ostream& os, const VarExpression &exp);
        };

        struct PrimitiveType : LocationInfo {
            std::string type; // "int" or "bool"
        public:
            friend std::ostream& operator<<(std::ostream& os, const PrimitiveType &exp); 
        };

        struct Expression : public boost::spirit::x3::variant<int, boost::spirit::x3::forward_ast<BinopExps>, bool, VarExpression, boost::spirit::x3::forward_ast<ExpressionPar>, boost::spirit::x3::forward_ast<FunctionCall>, boost::spirit::x3::forward_ast<ArrayExp>, boost::spirit::x3::forward_ast<ArrayIndex>, boost::spirit::x3::forward_ast<ObjInst>, IdAccess>, LocationInfo {
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

        struct ArrayIndex : LocationInfo {
            IdAccess idAccess;
            std::vector<Expression> indices; 
        public: 
            friend std::ostream& operator<<(std::ostream& os, const ArrayIndex &exp);
        };

        struct ArrayIndexAssign : LocationInfo {
            ArrayIndex index;
            Expression exp;
        public: 
            friend std::ostream& operator<<(std::ostream& os, const ArrayIndexAssign &exp);
        };

        struct Rhs : LocationInfo {
            std::string op;
            Expression exp;
            SymbolTable *scope = nullptr;
        public: 
            friend std::ostream& operator<<(std::ostream& os, const Rhs &exp);
        };

        struct BinopExps : LocationInfo { 
            Expression lhs;
            std::vector<Rhs> rhss;
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
            IdAccess idAccess; 
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const VarAssign &exp);
        };

        struct WhileStatement : LocationInfo {
            Expression exp; 
            boost::spirit::x3::forward_ast<Block> block;
        public:
            friend std::ostream& operator<<(std::ostream& os, const WhileStatement &exp);
        };

        struct StatementExpression : LocationInfo {
            Expression exp;
        public:
            friend std::ostream& operator<<(std::ostream& os, const StatementExpression &exp);
        };

        struct Statement : public boost::spirit::x3::variant<VarAssign, boost::spirit::x3::forward_ast<VarDeclAssign>, boost::spirit::x3::forward_ast<VarDeclStatement>, WhileStatement,StatementExpression, ArrayIndexAssign, PrintStatement, ReturnStatement, boost::spirit::x3::forward_ast<ConditionalStatement>, BreakStatement, ContinueStatement>, LocationInfo {
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

        struct BlockLine : public boost::spirit::x3::variant<boost::spirit::x3::forward_ast<Decl> , Statement>, LocationInfo  {
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
            boost::optional<ElseStatement> conditionalElse;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ConditionalStatement &exp);
        };

        struct ArrayType : LocationInfo {
            PrimitiveType type;
            int dim;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ArrayType &exp);
        };

        struct ClassType : LocationInfo {
            Id id;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ArrayType &exp);
        };

        struct Type : public boost::spirit::x3::variant<PrimitiveType, ArrayType, ClassType>, LocationInfo {
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

        struct VarDeclStatement : public boost::spirit::x3::variant<VarDecl> , LocationInfo {
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
        struct Parameter : public boost::spirit::x3::variant<VarDecl>, LocationInfo {
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

        struct ClassDecl : public LocationInfo {
            Id id;
            std::vector<VarDeclStatement> attr; 
            ClassSymbol *scope = nullptr;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ClassDecl &exp);
        };

        struct ObjInst : public LocationInfo {
            Id id;
            ArgumentList arguments;
        public:
            friend std::ostream& operator<<(std::ostream& os, const ObjInst &exp);
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
        

        struct ArrayExp : LocationInfo {
            PrimitiveType primType;
            std::vector<Expression> sizes; 
        public: 
            friend std::ostream& operator<<(std::ostream& os, const ArrayExp &exp);
        };


        struct Decl : public boost::spirit::x3::variant<VarDeclAssign, VarDeclStatement, FuncDecl, ClassDecl>, LocationInfo { 
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


#endif
