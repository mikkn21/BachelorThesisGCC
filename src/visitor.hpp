
#ifndef MGRAMMAR_VISITOR_HPP
#define MGRAMMAR_VISITOR_HPP 

#include <boost/variant.hpp>
#include "ast.hpp"



class Visitor {
public: 
    Visitor() { }

    virtual void preVisit(grammar::ast::Prog &prog) { }
    virtual void postVisit(grammar::ast::Prog &prog) { }

    virtual void preVisit(grammar::ast::PrimitiveType &type) { }
    virtual void postVisit(grammar::ast::PrimitiveType &type) { }

    virtual void preVisit(grammar::ast::Type &type) { }
    virtual void postVisit(grammar::ast::Type &type) { }

    virtual void preVisit(int &value) { }
    virtual void postVisit(int &value) { }

    virtual void preVisit(bool &value) { }
    virtual void postVisit(bool &value) { }

    virtual void preVisit(grammar::ast::BinopExps &exp) { }
    virtual void preRhsVisit(grammar::ast::BinopExps &exp) { }
    virtual void postVisit(grammar::ast::BinopExps &exp) { }

    virtual void preVisit(grammar::ast::Rhs &rhs) { }
    virtual void postVisit(grammar::ast::Rhs &rhs) { }

    virtual void preVisit(grammar::ast::VarExpression &exp) { }
    virtual void postVisit(grammar::ast::VarExpression &exp) { }

    virtual void preVisit(grammar::ast::FunctionCall &funcCall) { }
    virtual void preArgumentListVisit(grammar::ast::FunctionCall &funcCall) { }
    virtual void postVisit(grammar::ast::FunctionCall &funcCall) { }

    virtual void preVisit(grammar::ast::ArgumentList &exp) { }
    virtual void postVisit(grammar::ast::ArgumentList &exp) { }

    virtual void preVisit(grammar::ast::Id &id) { }
    virtual void postVisit(grammar::ast::Id &id) { }

    virtual void preVisit(grammar::ast::ExpressionPar &exp) { }
    virtual void postVisit(grammar::ast::ExpressionPar &exp) { }

    virtual void preVisit(grammar::ast::VarAssign &assign) { }
    virtual void preExpVisit(grammar::ast::VarAssign &assign) { }
    virtual void postVisit(grammar::ast::VarAssign &assign) { }

    virtual void preVisit(grammar::ast::WhileStatement &whileStatement) { }
    virtual void preBlockVisit(grammar::ast::WhileStatement &whileStatement) { }
    virtual void postVisit(grammar::ast::WhileStatement &whileStatement) { }

    virtual void preVisit(grammar::ast::StatementExpression &exp) { }
    virtual void postVisit(grammar::ast::StatementExpression &exp) { }

    virtual void preVisit(grammar::ast::Block &block) { }
    virtual void postVisit(grammar::ast::Block &block) { }

    virtual void preVisit(grammar::ast::IfStatement &ifStatement) { }
    virtual void preBlockVisit(grammar::ast::IfStatement &ifStatement) { }
    virtual void postVisit(grammar::ast::IfStatement &ifStatement) { }

    virtual void preVisit(grammar::ast::ElseStatement &elseStatement) { }
    virtual void postVisit(grammar::ast::ElseStatement &elseStatement) { }

    virtual void preVisit(grammar::ast::ConditionalStatement &conditional) { }
    virtual void preElseVisit(grammar::ast::ConditionalStatement &conditional) { }
    virtual void postVisit(grammar::ast::ConditionalStatement &conditional) { }

    virtual void preVisit(grammar::ast::Parameter &parameter) { }
    virtual void postVisit(grammar::ast::Parameter &parameter) { }

    virtual void preVisit(grammar::ast::ParameterList &list) { }
    virtual void postVisit(grammar::ast::ParameterList &list) { }

    virtual void preVisit(grammar::ast::VarDecl &decl) { }
    virtual void preIdVisit(grammar::ast::VarDecl &decl) { }
    virtual void postVisit(grammar::ast::VarDecl &decl) { }

    virtual void preVisit(grammar::ast::VarDeclAssign &decl) { }
    virtual void preExpVisit(grammar::ast::VarDeclAssign &decl) { }
    virtual void postVisit(grammar::ast::VarDeclAssign &decl) { }

    virtual void preVisit(grammar::ast::VarDeclStatement &decl) { }
    virtual void postVisit(grammar::ast::VarDeclStatement &decl) { }

    virtual void preVisit(grammar::ast::FuncDecl &decl) { }
    virtual void preIdVisit(grammar::ast::FuncDecl &decl) { }
    virtual void preParameterListVisit(grammar::ast::FuncDecl &decl) { }
    virtual void preBlockVisit(grammar::ast::FuncDecl &decl) { }
    virtual void postVisit(grammar::ast::FuncDecl &decl) { }

    virtual void preVisit(grammar::ast::PrintStatement &print) { }
    virtual void postVisit(grammar::ast::PrintStatement &print) { }

    virtual void preVisit(grammar::ast::ReturnStatement &ret) { }
    virtual void postVisit(grammar::ast::ReturnStatement &ret) { }

    virtual void preVisit(grammar::ast::BlockLine &blockLine) { }
    virtual void postVisit(grammar::ast::BlockLine &blockLine) { }
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
    void operator()(T &t);
};

template <>
void TreeTraveler::operator()(grammar::ast::Prog &prog);

#endif
