
#ifndef MGRAMMAR_VISITOR_HPP
#define MGRAMMAR_VISITOR_HPP 

#include <boost/variant.hpp>
#include "ast.hpp"



class Visitor {
public: 
    Visitor() { }

    virtual void pre_visit(grammar::ast::Prog &prog) { }
    virtual void post_visit(grammar::ast::Prog &prog) { }

    virtual void pre_visit(grammar::ast::PrimitiveType &type) { }
    virtual void post_visit(grammar::ast::PrimitiveType &type) { }

    virtual void pre_visit(grammar::ast::Type &type) { }
    virtual void post_visit(grammar::ast::Type &type) { }

    virtual void pre_visit(grammar::ast::ClassType &type) { }
    virtual void post_visit(grammar::ast::ClassType &type) { }

    virtual void pre_visit(grammar::ast::ClassDecl &class_decl) {}
    virtual void pre_id_visit(grammar::ast::ClassDecl &class_decl) {} 
    virtual void post_visit(grammar::ast::ClassDecl &class_decl) {}

    virtual void pre_visit(grammar::ast::ObjInst &obj_inst) {}
    virtual void pre_id_visit(grammar::ast::ObjInst &obj_inst) {} 
    virtual void post_visit(grammar::ast::ObjInst &obj_inst) {}

    
    virtual void pre_visit(grammar::ast::IdAccess &id_access) {}
    virtual void post_visit(grammar::ast::IdAccess &id_access) {}


    virtual void pre_visit(grammar::ast::ArrayType &type) {}
    virtual void preIntVisit(grammar::ast::ArrayType &type) {} 
    virtual void post_visit(grammar::ast::ArrayType &type) {}

    virtual void pre_visit(grammar::ast::ArrayIndexAssign &type) {}
    virtual void preArrayIndexVisit(grammar::ast::ArrayIndexAssign &type) {} 
    virtual void post_visit(grammar::ast::ArrayIndexAssign &type) {}

    virtual void pre_visit(grammar::ast::ArrayExp &exp) {}
    virtual void preSizeVisit(grammar::ast::ArrayExp &exp) {}
    virtual void post_visit(grammar::ast::ArrayExp &exp) {}
 
    virtual void pre_visit(grammar::ast::ArrayIndex &array_index) {}
    virtual void preIndexVisit(grammar::ast::ArrayIndex &array_index) {}
    virtual void post_visit(grammar::ast::ArrayIndex &array_index) {}

    virtual void pre_visit(int &value) { }
    virtual void post_visit(int &value) { }

    virtual void pre_visit(bool &value) { }
    virtual void post_visit(bool &value) { }

    virtual void pre_visit(grammar::ast::BinopExps &exp) { }
    virtual void preRhsVisit(grammar::ast::BinopExps &exp) { }
    virtual void post_visit(grammar::ast::BinopExps &exp) { }

    virtual void pre_visit(grammar::ast::Rhs &rhs) { }
    virtual void post_visit(grammar::ast::Rhs &rhs) { }

    virtual void pre_visit(grammar::ast::BreakStatement &brk) { }
    virtual void post_visit(grammar::ast::BreakStatement &brk) { }

    virtual void pre_visit(grammar::ast::ContinueStatement &cont) { }
    virtual void post_visit(grammar::ast::ContinueStatement &cont) { }

    virtual void pre_visit(grammar::ast::VarExpression &exp) { }
    virtual void post_visit(grammar::ast::VarExpression &exp) { }

    virtual void pre_visit(grammar::ast::FunctionCall &func_call) { }
    virtual void preArgumentListVisit(grammar::ast::FunctionCall &func_call) { }
    virtual void post_visit(grammar::ast::FunctionCall &func_call) { }

    virtual void pre_visit(grammar::ast::ArgumentList &exp) { }
    virtual void post_visit(grammar::ast::ArgumentList &exp) { }

    virtual void pre_visit(grammar::ast::Id &id) { }
    virtual void post_visit(grammar::ast::Id &id) { }

    virtual void pre_visit(grammar::ast::ExpressionPar &exp) { }
    virtual void post_visit(grammar::ast::ExpressionPar &exp) { }

    virtual void pre_visit(grammar::ast::VarAssign &assign) { }
    virtual void pre_exp_visit(grammar::ast::VarAssign &assign) { }
    virtual void post_visit(grammar::ast::VarAssign &assign) { }

    virtual void pre_visit(grammar::ast::WhileStatement &while_statement) { }
    virtual void pre_block_visit(grammar::ast::WhileStatement &while_statement) { }
    virtual void post_visit(grammar::ast::WhileStatement &while_statement) { }

    virtual void pre_visit(grammar::ast::StatementExpression &exp) { }
    virtual void post_visit(grammar::ast::StatementExpression &exp) { }

    virtual void pre_visit(grammar::ast::Block &block) { }
    virtual void post_visit(grammar::ast::Block &block) { }

    virtual void pre_visit(grammar::ast::IfStatement &if_statement) { }
    virtual void pre_block_visit(grammar::ast::IfStatement &if_statement) { }
    virtual void post_visit(grammar::ast::IfStatement &if_statement) { }

    virtual void pre_visit(grammar::ast::ElseStatement &else_statement) { }
    virtual void post_visit(grammar::ast::ElseStatement &else_statement) { }

    virtual void pre_visit(grammar::ast::ConditionalStatement &conditional) { }
    virtual void preElseVisit(grammar::ast::ConditionalStatement &conditional) { }
    virtual void post_visit(grammar::ast::ConditionalStatement &conditional) { }

    virtual void pre_visit(grammar::ast::Parameter &parameter) { }
    virtual void post_visit(grammar::ast::Parameter &parameter) { }

    virtual void pre_visit(grammar::ast::ParameterList &list) { }
    virtual void post_visit(grammar::ast::ParameterList &list) { }

    virtual void pre_visit(grammar::ast::VarDecl &decl) { }
    virtual void pre_id_visit(grammar::ast::VarDecl &decl) { }
    virtual void post_visit(grammar::ast::VarDecl &decl) { }

    virtual void pre_visit(grammar::ast::VarDeclAssign &decl) { }
    virtual void pre_exp_visit(grammar::ast::VarDeclAssign &decl) { }
    virtual void post_visit(grammar::ast::VarDeclAssign &decl) { }

    virtual void pre_visit(grammar::ast::VarDeclStatement &decl) { }
    virtual void post_visit(grammar::ast::VarDeclStatement &decl) { }

    virtual void pre_visit(grammar::ast::FuncDecl &decl) { }
    virtual void pre_id_visit(grammar::ast::FuncDecl &decl) { }
    virtual void pre_parameter_list_visit(grammar::ast::FuncDecl &decl) { }
    virtual void pre_block_visit(grammar::ast::FuncDecl &decl) { }
    virtual void post_visit(grammar::ast::FuncDecl &decl) { }

    virtual void pre_visit(grammar::ast::PrintStatement &print) { }
    virtual void post_visit(grammar::ast::PrintStatement &print) { }

    virtual void pre_visit(grammar::ast::ReturnStatement &ret) { }
    virtual void post_visit(grammar::ast::ReturnStatement &ret) { }

    virtual void pre_visit(grammar::ast::BlockLine &block_line) { }
    virtual void post_visit(grammar::ast::BlockLine &block_line) { }
};

class TreeTraveler : boost::static_visitor<> {
private: 
    Visitor &visitor;

public:
    TreeTraveler(Visitor &visitor) : visitor(visitor) { }

    template <typename T>
    void operator()(boost::spirit::x3::forward_ast<T> &ast);

    template <typename T>
    void operator()(boost::optional<T> &ast);

    template <typename T>
    void operator()(std::vector<T> &t);

    template <typename T>
    void operator()(T &t);
};

template <>
void TreeTraveler::operator()(grammar::ast::Prog &prog);

#endif
