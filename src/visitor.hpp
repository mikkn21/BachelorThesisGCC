
#ifndef MGRAMMAR_VISITOR_HPP
#define MGRAMMAR_VISITOR_HPP 

#include <boost/variant.hpp>
#include "ast.hpp"

namespace x3 = boost::spirit::x3;

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
    void operator()(x3::forward_ast<T> &ast);

    template <typename T>
    void operator()(T &t);
};

template <>
void TreeTraveler::operator()(Prog &prog);

#endif
