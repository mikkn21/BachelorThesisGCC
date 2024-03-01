
#include "visitor.hpp"
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <iostream>

using boost::apply_visitor;

// Types
template <>
void TreeTraveler::operator()(PrimitiveType &type) {

}

// Expressions
template <>
void TreeTraveler::operator()(int &value) {

}

template <>
void TreeTraveler::operator()(bool &value) {

}

template <>
void TreeTraveler::operator()(x3::forward_ast<BinopExp> &binop) {

}

template <>
void TreeTraveler::operator()(Id &id) {

}

template <>
void TreeTraveler::operator()(x3::forward_ast<ExpressionPar> &expPar) {

}

template <>
void TreeTraveler::operator()(VarAssign &varAssign) {

}

template <>
void TreeTraveler::operator()(WhileStatement &whileStatement) {
    
}

template <>
void TreeTraveler::operator()(StatementExpression &statement) {
    apply_visitor(*this, statement.exp);
}

template <>
void TreeTraveler::operator()(Block &block) {
    for (auto const &blockLine : block.block_line) {
        // apply_visitor(*this, blockLine);
    }
}

template <>
void TreeTraveler::operator()(Parameter &parameter) {
    apply_visitor(*this, parameter.type);
    (*this)(parameter.id);
}

template <>
void TreeTraveler::operator()(ParameterList &parameterList) {
    for (auto const &parameter : parameterList.parameter) {
        //(*this)(parameter);
    }
}

template <>
void TreeTraveler::operator()(VarDecl &decl) {
    visitor.preVarDecl(decl);
    apply_visitor(*this, decl.type);
    (*this)(decl.id);
    apply_visitor(*this, decl.exp);
}

template <>
void TreeTraveler::operator()(FuncDecl &decl) {
    visitor.preFuncDecl(decl);
    apply_visitor(*this, decl.type);
    (*this)(decl.id);
    (*this)(decl.parameter_list);
    (*this)(decl.block);
    visitor.postFuncDecl(decl);
}

template <>
void TreeTraveler::operator()(Prog &prog) {
    this->visitor.progPreDecl(prog);
    for (auto const &decl : prog.decls) {
        //apply_visitor(*this, decl);
    }
    this->visitor.progPostDecl(prog);
}
