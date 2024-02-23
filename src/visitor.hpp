
#ifndef MGRAMMAR_VISITOR_HPP
#define MGRAMMAR_VISITOR_HPP 

#include "ast.hpp"

class Visitor {
public:
    virtual void visitProgPreDecl(const Prog &prog) {}
    virtual void visitProgPostDecl(const Prog &prog) {}
    virtual void visitVarAssignPostId(const Decl &decl) {}
    virtual void visitVarAssignPreId(const VarAssign &varAssign) {}
};

template <typename T>
void astAccept(T element, Visitor visitor);

#endif
