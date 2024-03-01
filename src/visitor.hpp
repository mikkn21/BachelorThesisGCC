
#ifndef MGRAMMAR_VISITOR_HPP
#define MGRAMMAR_VISITOR_HPP 

#include <boost/variant.hpp>
#include "ast.hpp"

class Visitor {
public: 
    Visitor() { }

    virtual void progPreDecl(Prog &prog) { }
    virtual void progPostDecl(Prog &prog) { }
    virtual void preVarDecl(VarDecl &varDecl) { }
    virtual void preFuncDecl(FuncDecl &funcDecl) { }
    virtual void postFuncDecl(FuncDecl &funcDecl) { }
};

class TreeTraveler : boost::static_visitor<> {
private: 
    Visitor &visitor;

public:
    TreeTraveler(Visitor &visitor) : visitor(visitor) { }

    template <typename T>
    void operator()(T &t);
};

template <>
void TreeTraveler::operator()(Id &id);

template <>
void TreeTraveler::operator()(VarDecl &varDecl);

template <>
void TreeTraveler::operator()(FuncDecl &decl);

template <>
void TreeTraveler::operator()(Prog &prog);

#endif
