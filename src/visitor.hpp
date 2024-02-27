
#ifndef MGRAMMAR_VISITOR_HPP
#define MGRAMMAR_VISITOR_HPP 

#include <boost/variant.hpp>
#include "ast.hpp"

class Visitor {
public: 
    Visitor() { }

    virtual void progPreDecl(const Prog &prog) { }
    virtual void progPostDecl(const Prog &prog) { }
    virtual void preVarDecl(const VarDecl &varDecl) { }
    virtual void preFuncDecl(const FuncDecl &funcDecl) { }
    virtual void postFuncDecl(const FuncDecl &funcDecl) { }
};

class TreeTraveler : boost::static_visitor<> {
private: 
    Visitor &visitor;

public:
    TreeTraveler(Visitor &visitor) : visitor(visitor) { }

    template <typename T>
    void operator()(const T &t) { }
};

template <>
void TreeTraveler::operator()(const Type &type);

template <>
void TreeTraveler::operator()(const Id &id);

template <>
void TreeTraveler::operator()(const VarDecl &varDecl);

template <>
void TreeTraveler::operator()(const FuncDecl &varDecl);

template <>
void TreeTraveler::operator()(const Prog &prog);

#endif
