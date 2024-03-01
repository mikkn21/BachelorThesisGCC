
#include "visitor.hpp"
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <iostream>

using boost::apply_visitor;

// Types
template <>
void TreeTraveler::operator()(const PrimitiveType &type) {

}

// Expressions
template <>
void TreeTraveler::operator()(const int &value) {

}

template <>
void TreeTraveler::operator()(const bool &value) {

}

template <>
void TreeTraveler::operator()(const x3::forward_ast<BinopExp> &binop) {

}

template <>
void TreeTraveler::operator()(const Id &id) {

}

template <>
void TreeTraveler::operator()(const x3::forward_ast<ExpressionPar> &expPar) {

}

template <>
void TreeTraveler::operator()(const VarAssign &varAssign) {

}

template <>
void TreeTraveler::operator()(const WhileStatement &whileStatement) {
    
}

template <>
void TreeTraveler::operator()(const StatementExpression &statement) {
    apply_visitor(*this, statement.exp);
}

template <>
void TreeTraveler::operator()(const Block &block) {
    for (auto const &blockLine : block.block_line) {
        // apply_visitor(*this, blockLine);
    }
}

template <>
void TreeTraveler::operator()(const Parameter &parameter) {
    apply_visitor(*this, parameter.type);
    (*this)(parameter.id);
}

template <>
void TreeTraveler::operator()(const ParameterList &parameterList) {
    for (auto const &parameter : parameterList.parameter) {
        (*this)(parameter);
    }
}

template <>
void TreeTraveler::operator()(const VarDecl &decl) {
    visitor.preVarDecl(decl);
    apply_visitor(*this, decl.type);
    (*this)(decl.id);
    apply_visitor(*this, decl.exp);
}

template <>
void TreeTraveler::operator()(const FuncDecl &decl) {
    visitor.preFuncDecl(decl);
    apply_visitor(*this, decl.type);
    (*this)(decl.id);
    (*this)(decl.parameter_list);
    (*this)(decl.block);
    visitor.postFuncDecl(decl);
}

template <>
void TreeTraveler::operator()(const Prog &prog) {
    this->visitor.progPreDecl(prog);
    for (auto const &decl : prog.decls) {
        apply_visitor(*this, decl);
    }
    this->visitor.progPostDecl(prog);
}
