
#include "visitor.hpp"
#include "ast.hpp"
//#include "error/compiler_error.hpp"
#include <variant>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <iostream>

using boost::apply_visitor;
using namespace std;
namespace x3 = boost::spirit::x3;


// --- Adding new implementations for AST nodes ---
// Variants need to be added at the top with the other varionts and have the
// same implementation as the others.
//
// The others need to be added below anywhere. Look at the other implementations
// for "inpiration".


// Handle forward AST by calling the operator on the undelying object.
template <typename T>
void TreeTraveler::operator()(x3::forward_ast<T> &ast) {
    (*this)(ast.get());
}

template <typename T>
void TreeTraveler::operator()(boost::optional<T> &opt) {
    if (opt.has_value()) {
        (*this)(opt.value());
    }
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
    visitor.preVisit(blockLine);
    apply_visitor(*this, blockLine);
    visitor.postVisit(blockLine);
}

template <>
void TreeTraveler::operator()(Type &type) {
    visitor.preVisit(type);
    apply_visitor(*this, type);
    visitor.postVisit(type);
}

template <>
void TreeTraveler::operator()(VarDeclStatement &decl) {
    visitor.preVisit(decl);
    apply_visitor(*this, decl);
    visitor.postVisit(decl);
}

template <>
void TreeTraveler::operator()(Parameter &parameter) {
    visitor.preVisit(parameter);
    apply_visitor(*this, parameter);
    visitor.postVisit(parameter);
}

// Types
template <>
void TreeTraveler::operator()(PrimitiveType &type) {
    visitor.preVisit(type);
    visitor.postVisit(type);
}

// Expressions
template <>
void TreeTraveler::operator()(int &value) {
    visitor.preVisit(value);
    visitor.postVisit(value);
}

template <>
void TreeTraveler::operator()(bool &value) {
    visitor.preVisit(value);
    visitor.postVisit(value);
}

template <>
void TreeTraveler::operator()(Rhs &rhs) {
    visitor.preVisit(rhs);
    // The operator is not visited because it's a string
    (*this)(rhs.exp);
    visitor.postVisit(rhs);
}

template <>
void TreeTraveler::operator()(BinopExps &binop) {
    visitor.preVisit(binop);
    (*this)(binop.lhs);
    visitor.preRhsVisit(binop);
    for(auto &rhs : binop.rhss) {
        (*this)(rhs);
    }
    visitor.postVisit(binop);
}

template <>
void TreeTraveler::operator()(Id &id) {
    visitor.preVisit(id);
    visitor.postVisit(id);
}

template <>
void TreeTraveler::operator()(VarExpression &exp) {
    visitor.preVisit(exp);
    (*this)(exp.id);
    visitor.postVisit(exp);
}

template <>
void TreeTraveler::operator()(ExpressionPar &expPar) {
    visitor.preVisit(expPar);
    (*this)(expPar.exp);
    visitor.postVisit(expPar);
}

template <>
void TreeTraveler::operator()(ArgumentList &argList) {
    visitor.preVisit(argList);
    for (auto &arg : argList.arguments) {
        (*this)(arg);
    }
    visitor.postVisit(argList);
}

template <>
void TreeTraveler::operator()(FunctionCall &funcCall) {
    visitor.preVisit(funcCall);
    (*this)(funcCall.id);
    visitor.preArgumentListVisit(funcCall);
    (*this)(funcCall.argument_list);
    visitor.postVisit(funcCall);
}

// Statements
template <>
void TreeTraveler::operator()(VarAssign &varAssign) {
    visitor.preVisit(varAssign);
    (*this)(varAssign.id);
    visitor.preExpVisit(varAssign);
    (*this)(varAssign.exp);
    visitor.postVisit(varAssign);
}

template <>
void TreeTraveler::operator()(Block &block) {
    visitor.preVisit(block);
    for (auto &blockLine : block.block_line) {
        (*this)(blockLine);
    }
    visitor.postVisit(block);
}

template <>
void TreeTraveler::operator()(IfStatement &statement) {
    visitor.preVisit(statement);
    (*this)(statement.exp);
    visitor.preBlockVisit(statement);
    (*this)(statement.block);
    visitor.postVisit(statement);
}

template <>
void TreeTraveler::operator()(ElseStatement &statement) {
    visitor.preVisit(statement);
    (*this)(statement.block);
    visitor.postVisit(statement);
}

template <>
void TreeTraveler::operator()(ConditionalStatement &statement) {
    visitor.preVisit(statement);
    (*this)(statement.ifStatement);
    for (auto ifStatement : statement.elseIfs) {
        (*this)(ifStatement);
    }
    visitor.preElseVisit(statement);
    (*this)(statement.conditionalElse);
    visitor.postVisit(statement);
}

template <>
void TreeTraveler::operator()(PrintStatement &print) {
    visitor.preVisit(print);
    (*this)(print.exp);
    visitor.postVisit(print);
}

template <>
void TreeTraveler::operator()(ReturnStatement &ret) {
    visitor.preVisit(ret);
    (*this)(ret.exp);
    visitor.postVisit(ret);
}

template <>
void TreeTraveler::operator()(WhileStatement &whileStatement) {
    visitor.preVisit(whileStatement);
    (*this)(whileStatement.exp);
    visitor.preBlockVisit(whileStatement);
    (*this)(whileStatement.block);
    visitor.postVisit(whileStatement);
}

template <>
void TreeTraveler::operator()(StatementExpression &statement) {
    visitor.preVisit(statement);
    (*this)(statement.exp);
    visitor.postVisit(statement);
}


template <>
void TreeTraveler::operator()(VarDecl &decl) {
    visitor.preVisit(decl);
    (*this)(decl.type);
    visitor.preIdVisit(decl);
    (*this)(decl.id);
    visitor.postVisit(decl);
}

template <>
void TreeTraveler::operator()(VarDeclAssign &decl) {
    visitor.preVisit(decl);
    (*this)(decl.decl);
    visitor.preExpVisit(decl);
    (*this)(decl.exp);
    visitor.postVisit(decl);
}

template <>
void TreeTraveler::operator()(ParameterList &parameterList) {
    visitor.preVisit(parameterList);
    for (auto &parameter : parameterList.parameter) {
        (*this)(parameter);
    }
    visitor.postVisit(parameterList);
}

template <>
void TreeTraveler::operator()(FuncDecl &decl) {
    visitor.preVisit(decl);
    (*this)(decl.type);
    visitor.preIdVisit(decl);
    (*this)(decl.id);
    visitor.preParameterListVisit(decl);
    (*this)(decl.parameter_list);
    visitor.preBlockVisit(decl);
    (*this)(decl.block);
    visitor.postVisit(decl);
}

template <>
void TreeTraveler::operator()(Prog &prog) {
    visitor.preVisit(prog);
    for (auto &decl : prog.decls) {
        (*this)(decl);
    }
    visitor.postVisit(prog);
}

