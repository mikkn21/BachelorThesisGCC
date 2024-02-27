
#include "visitor.hpp"
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <iostream>

using boost::apply_visitor;


template <>
void TreeTraveler::operator()(const Type &type) {
    
}

template <>
void TreeTraveler::operator()(const Id &id) {

}

template <>
void TreeTraveler::operator()(const VarDecl &decl) {
    visitor.preVarDecl(decl);
    (*this)(decl.type);
    (*this)(decl.id);
    apply_visitor(*this, decl.exp);
}

template <>
void TreeTraveler::operator()(const FuncDecl &decl) {
    visitor.preFuncDecl(decl);
    (*this)(decl.type);
    (*this)(decl.id);
    (*this)(decl.parameter_list);
    (*this)(decl.block);
    visitor.postFuncDecl(decl);
}

template <>
void TreeTraveler::operator()(const Prog &prog) {
    this->visitor.progPreDecl(prog);
    apply_visitor(*this, prog.decl);
    this->visitor.progPostDecl(prog);
}
