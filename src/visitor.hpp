
#ifndef MGRAMMAR_VISITOR_HPP
#define MGRAMMAR_VISITOR_HPP 

#include <boost/variant.hpp>
#include "ast.hpp"

namespace x3 = boost::spirit::x3;

class Visitor {
public: 
    Visitor() { }

    virtual void preVisit(Prog &prog) { }
    virtual void postVisit(Prog &prog) { }

    virtual void preVisit(PrimitiveType &type) { }
    virtual void postVisit(PrimitiveType &type) { }

    virtual void preVisit(int &value) { }
    virtual void postVisit(int &value) { }

    virtual void preVisit(bool &value) { }
    virtual void postVisit(bool &value) { }

    virtual void preVisit(BinopExp &exp) { }
    virtual void preRhsVisit(BinopExp &exp) { }
    virtual void postVisit(BinopExp &exp) { }

    virtual void preVisit(Id &id) { }
    virtual void postVisit(Id &id) { }

    virtual void preVisit(ExpressionPar &exp) { }
    virtual void postVisit(ExpressionPar &exp) { }

    virtual void preVisit(VarAssign &assign) { }
    virtual void preExpVisit(VarAssign &assign) { }
    virtual void postVisit(VarAssign &assign) { }

    virtual void preVisit(WhileStatement &whileStatement) { }
    virtual void preBlockVisit(WhileStatement &whileStatement) { }
    virtual void postVisit(WhileStatement &whileStatement) { }

    virtual void preVisit(StatementExpression &exp) { }
    virtual void postVisit(StatementExpression &exp) { }

    virtual void preVisit(Block &block) { }
    virtual void postVisit(Block &block) { }

    virtual void preVisit(Parameter &parameter) { }
    virtual void preIdVisit(Parameter &parameter) { }
    virtual void postVisit(Parameter &parameter) { }

    virtual void preVisit(ParameterList &list) { }
    virtual void postVisit(ParameterList &list) { }

    virtual void preVisit(VarDecl &decl) { }
    virtual void preIdVisit(VarDecl &decl) { }
    virtual void preExpVisit(VarDecl &decl) { }
    virtual void postVisit(VarDecl &decl) { }

    virtual void preVisit(FuncDecl &decl) { }
    virtual void preIdVisit(FuncDecl &decl) { }
    virtual void preParameterListVisit(FuncDecl &decl) { }
    virtual void preBlockVisit(FuncDecl &decl) { }
    virtual void postVisit(FuncDecl &decl) { }

    virtual void preVisit(PrintStatement &print) { }
    virtual void postVisit(PrintStatement &print) { }

    virtual void preVisit(ReturnStatement &ret) { }
    virtual void postVisit(ReturnStatement &ret) { }

    virtual void preVisit(BlockLine &blockLine) { }
    virtual void postVisit(BlockLine &blockLine) { }
};

class TreeTraveler : boost::static_visitor<> {
private: 
    Visitor &visitor;

public:
    TreeTraveler(Visitor &visitor) : visitor(visitor) { }

    template <typename T>
    void operator()(x3::forward_ast<T> &ast);

    template <typename T>
    void operator()(T &t);
};

template <>
void TreeTraveler::operator()(Prog &prog);

#endif
