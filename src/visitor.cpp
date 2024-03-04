
#include "visitor.hpp"
#include "compiler_error.hpp"
#include <variant>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <iostream>

using boost::apply_visitor;
using namespace std;
namespace x3 = boost::spirit::x3;


// Forward AST
template <typename T>
void TreeTraveler::operator()(x3::forward_ast<T> &ast) {
    (*this)(ast.get());
}

// Variants
template <>
void TreeTraveler::operator()(Decl &decl) {
    apply_visitor(*this, decl);
}

template <>
void TreeTraveler::operator()(Statement &statement) {
    apply_visitor(*this, statement);
}

template <>
void TreeTraveler::operator()(Expression &exp) {
    apply_visitor(*this, exp);
}

template <>
void TreeTraveler::operator()(BlockLine &blockLine) {
    apply_visitor(*this, blockLine);
}

template <>
void TreeTraveler::operator()(Type &type) {
    apply_visitor(*this, type);
}

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
void TreeTraveler::operator()(BinopExp &binop) {
    (*this)(binop.lhs);
    (*this)(binop.rhs);
}

template <>
void TreeTraveler::operator()(Id &id) {

}

template <>
void TreeTraveler::operator()(ExpressionPar &expPar) {
    (*this)(expPar.exp);
}

// Statements
template <>
void TreeTraveler::operator()(VarAssign &varAssign) {
    (*this)(varAssign.id);
    (*this)(varAssign.exp);
}

template <>
void TreeTraveler::operator()(WhileStatement &whileStatement) {
    (*this)(whileStatement.exp);
    (*this)(whileStatement.block);
}

template <>
void TreeTraveler::operator()(StatementExpression &statement) {
    (*this)(statement.exp);
}

template <>
void TreeTraveler::operator()(Block &block) {
    for (auto &blockLine : block.block_line) {
        (*this)(blockLine);
    }
}

template <>
void TreeTraveler::operator()(Parameter &parameter) {
    (*this)(parameter.type);
    (*this)(parameter.id);
}

template <>
void TreeTraveler::operator()(ParameterList &parameterList) {
    for (auto &parameter : parameterList.parameter) {
        (*this)(parameter);
    }
}

template <>
void TreeTraveler::operator()(VarDecl &decl) {
    visitor.preVarDecl(decl);
    (*this)(decl.type);
    (*this)(decl.id);
    (*this)(decl.exp);
}

template <>
void TreeTraveler::operator()(FuncDecl &decl) {
    visitor.preFuncDecl(decl);
    (*this)(decl.type);
    (*this)(decl.id);
    (*this)(decl.parameter_list);
    (*this)(decl.block);
    visitor.postFuncDecl(decl);
}

template <>
void TreeTraveler::operator()(Prog &prog) {
    this->visitor.progPreDecl(prog);
    for (auto &decl : prog.decls) {
        (*this)(decl);
    }
    this->visitor.progPostDecl(prog);
}

