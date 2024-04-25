
#include "visitor.hpp"
#include "ast.hpp"
// Included in Visitor.hpp>ast.hpp #include <boost/spirit/home/x3/support/ast/variant.hpp>



// --- Adding new implementations for AST nodes ---
// Variants need to be added at the top with the other varionts and have the
// same implementation as the others.
//
// The others need to be added below anywhere. Look at the other implementations
// for "inpiration".


// Handle forward AST by calling the operator on the undelying object.
template <typename T>
void TreeTraveler::operator()(boost::spirit::x3::forward_ast<T> &ast) {
    (*this)(ast.get());
}

template <typename T>
void TreeTraveler::operator()(boost::optional<T> &opt) {
    if (opt.has_value()) {
        (*this)(opt.value());
    }
}

template <typename T>
void TreeTraveler::operator()(std::vector<T> &vec) {
    for (auto &elem : vec) {
        (*this)(elem);
    }
}

// Variants
template <>
void TreeTraveler::operator()(grammar::ast::Decl &decl) {
    apply_visitor(*this, decl);
}

template <>
void TreeTraveler::operator()(grammar::ast::Statement &statement) {
    apply_visitor(*this, statement);
}


template <>
void TreeTraveler::operator()(grammar::ast::Expression &exp) {
    apply_visitor(*this, exp);
}

template <>
void TreeTraveler::operator()(grammar::ast::BlockLine &block_line) {
    visitor.pre_visit(block_line);
    apply_visitor(*this, block_line);
    visitor.post_visit(block_line);
}

template <>
void TreeTraveler::operator()(grammar::ast::Type &type) {
    visitor.pre_visit(type);
    apply_visitor(*this, type);
    visitor.post_visit(type);
}

template <>
void TreeTraveler::operator()(grammar::ast::VarDeclStatement &decl) {
    visitor.pre_visit(decl);
    apply_visitor(*this, decl);
    visitor.post_visit(decl);
}

template <>
void TreeTraveler::operator()(grammar::ast::Parameter &parameter) {
    visitor.pre_visit(parameter);
    apply_visitor(*this, parameter);
    visitor.post_visit(parameter);
}

template <>
void TreeTraveler::operator()(grammar::ast::Id &id) {
    visitor.pre_visit(id);
    visitor.post_visit(id);
}

// Types
template <>
void TreeTraveler::operator()(grammar::ast::PrimitiveType &type) {
    visitor.pre_visit(type);
    visitor.post_visit(type);
}

template <>
void TreeTraveler::operator()(grammar::ast::ClassType &type) {
    visitor.pre_visit(type);
    (*this)(type.id);
    visitor.post_visit(type);
}

// Expressions
template <>
void TreeTraveler::operator()(int &value) {
    visitor.pre_visit(value);
    visitor.post_visit(value);
}

template <>
void TreeTraveler::operator()(bool &value) {
    visitor.pre_visit(value);
    visitor.post_visit(value);
}

template <>
void TreeTraveler::operator()(grammar::ast::Rhs &rhs) {
    visitor.pre_visit(rhs);
    // The operator is not visited because it's a string
    (*this)(rhs.exp);
    visitor.post_visit(rhs);
}

template <>
void TreeTraveler::operator()(grammar::ast::BinopExps &binop) {
    visitor.pre_visit(binop);
    (*this)(binop.lhs);
    visitor.preRhsVisit(binop);
    (*this)(binop.rhss);
    visitor.post_visit(binop);
}

template <>
void TreeTraveler::operator()(grammar::ast::IdAccess &id_access) {
  visitor.pre_visit(id_access);
  (*this)(id_access.ids);
  visitor.post_visit(id_access);
}

template <>
void TreeTraveler::operator()(grammar::ast::VarExpression &exp) {
    visitor.pre_visit(exp);
    (*this)(exp.id_access);
    visitor.post_visit(exp);
}

template <>
void TreeTraveler::operator()(grammar::ast::ExpressionPar &exp_par) {
    visitor.pre_visit(exp_par);
    (*this)(exp_par.exp);
    visitor.post_visit(exp_par);
}

template <>
void TreeTraveler::operator()(grammar::ast::ArgumentList &arg_list) {
    visitor.pre_visit(arg_list);
    (*this)(arg_list.arguments);
    visitor.post_visit(arg_list);
}

template <>
void TreeTraveler::operator()(grammar::ast::FunctionCall &func_call) {
    visitor.pre_visit(func_call);
    (*this)(func_call.id);
    visitor.preArgumentListVisit(func_call);
    (*this)(func_call.argument_list);
    visitor.post_visit(func_call);
}


template <>
void TreeTraveler::operator()(grammar::ast::ClassDecl &class_decl) {
    visitor.pre_visit(class_decl);
    (*this)(class_decl.id);
    visitor.pre_id_visit(class_decl); 
    (*this)(class_decl.attr);
    visitor.post_visit(class_decl);
}


// Arrays
template <>
void TreeTraveler::operator()(grammar::ast::ArrayType &array_type) {
    visitor.pre_visit(array_type);
    (*this)(array_type.type);
    visitor.preIntVisit(array_type);
    (*this)(array_type.dim);
    visitor.post_visit(array_type);
}

template <>
void TreeTraveler::operator()(grammar::ast::ArrayInitExp &arrayExp) {
    visitor.preVisit(arrayExp);
    (*this)(arrayExp.primType);
    visitor.preSizeVisit(arrayExp);
    (*this)(arrayExp.sizes);
    visitor.postVisit(arrayExp);
}

template <>
void TreeTraveler::operator()(grammar::ast::ArrayIndex &array_index) {
    visitor.pre_visit(array_index);
    (*this)(array_index.id_access);
    visitor.preIndexVisit(array_index);
    (*this)(array_index.indices);
    visitor.post_visit(array_index);
}

template <>
void TreeTraveler::operator()(grammar::ast::ArrayIndexAssign &array_index_assign) {
    visitor.pre_visit(array_index_assign);
    (*this)(array_index_assign.index);
    visitor.preArrayIndexVisit(array_index_assign);
    (*this)(array_index_assign.exp);
    visitor.post_visit(array_index_assign);
}

template <>
void TreeTraveler::operator()(grammar::ast::ArrayIndexExp &ArrayIndexExp) {
    visitor.preVisit(ArrayIndexExp);
    (*this)(ArrayIndexExp.index);
    visitor.postVisit(ArrayIndexExp);
}


// Statements
template <>
void TreeTraveler::operator()(grammar::ast::VarAssign &var_assign) {
    visitor.pre_visit(var_assign);
    (*this)(var_assign.id_access);
    visitor.pre_exp_visit(var_assign);
    (*this)(var_assign.exp);
    visitor.post_visit(var_assign);
}

template <>
void TreeTraveler::operator()(grammar::ast::Block &block) {
    visitor.pre_visit(block);
    (*this)(block.block_line);
    visitor.post_visit(block);
}

template <>
void TreeTraveler::operator()(grammar::ast::IfStatement &statement) {
    visitor.pre_visit(statement);
    (*this)(statement.exp);
    visitor.pre_block_visit(statement);
    (*this)(statement.block);
    visitor.post_visit(statement);
}

template <>
void TreeTraveler::operator()(grammar::ast::ElseStatement &statement) {
    visitor.pre_visit(statement);
    (*this)(statement.block);
    visitor.post_visit(statement);
}

template <>
void TreeTraveler::operator()(grammar::ast::ConditionalStatement &statement) {
    visitor.pre_visit(statement);
    (*this)(statement.if_statement);
    (*this)(statement.else_if);
    visitor.preElseVisit(statement);
    (*this)(statement.conditional_else);
    visitor.post_visit(statement);
}

template <>
void TreeTraveler::operator()(grammar::ast::PrintStatement &print) {
    visitor.pre_visit(print);
    (*this)(print.exp);
    visitor.post_visit(print);
}

template <>
void TreeTraveler::operator()(grammar::ast::ReturnStatement &ret) {
    visitor.pre_visit(ret);
    (*this)(ret.exp);
    visitor.post_visit(ret);
}

template <>
void TreeTraveler::operator()(grammar::ast::WhileStatement &while_statement) {
    visitor.pre_visit(while_statement);
    (*this)(while_statement.exp);
    visitor.pre_block_visit(while_statement);
    (*this)(while_statement.block);
    visitor.post_visit(while_statement);
}

template <> 
void TreeTraveler::operator()(grammar::ast::ContinueStatement &cont) {
    visitor.pre_visit(cont);
    visitor.post_visit(cont);
}

template <>
void TreeTraveler::operator()(grammar::ast::BreakStatement &brk) {
    visitor.pre_visit(brk);
    visitor.post_visit(brk);
}

template <>
void TreeTraveler::operator()(grammar::ast::StatementExpression &statement) {
    visitor.pre_visit(statement);
    (*this)(statement.exp);
    visitor.post_visit(statement);
}


template <>
void TreeTraveler::operator()(grammar::ast::VarDecl &decl) {
    visitor.pre_visit(decl);
    (*this)(decl.type);
    visitor.pre_id_visit(decl);
    (*this)(decl.id);
    visitor.post_visit(decl);
}

template <>
void TreeTraveler::operator()(grammar::ast::VarDeclAssign &decl) {
    visitor.pre_visit(decl);
    (*this)(decl.decl);
    visitor.pre_exp_visit(decl);
    (*this)(decl.exp);
    visitor.post_visit(decl);
}

template <>
void TreeTraveler::operator()(grammar::ast::ParameterList &parameter_list) {
    visitor.pre_visit(parameter_list);
    (*this)(parameter_list.parameter);
    visitor.post_visit(parameter_list);
}

template <>
void TreeTraveler::operator()(grammar::ast::ObjInst &obj_inst) {
  visitor.pre_visit(obj_inst);
  (*this)(obj_inst.id);
  visitor.pre_id_visit(obj_inst);
  (*this)(obj_inst.arguments); 
  visitor.post_visit(obj_inst);
}



template <>
void TreeTraveler::operator()(grammar::ast::FuncDecl &decl) {
    visitor.pre_visit(decl);
    (*this)(decl.type);
    visitor.pre_id_visit(decl);
    (*this)(decl.id);
    visitor.pre_parameter_list_visit(decl);
    (*this)(decl.parameter_list);
    visitor.pre_block_visit(decl);
    (*this)(decl.block);
    visitor.post_visit(decl);
}

template <>
void TreeTraveler::operator()(grammar::ast::Prog &prog) {
    visitor.pre_visit(prog);
    (*this)(prog.decls);
    visitor.post_visit(prog);
}

