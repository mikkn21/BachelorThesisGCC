#include "code_generation.hpp"
#include "../semantics/symbol_table.hpp"


#include <string>
// #include <boost/spirit/home/x3.hpp>
// #include <boost/spirit/home/x3/support/ast/apply_visitor.hpp>
#include <iostream>

IRVisitor::IRVisitor() : Visitor() {}

void IRVisitor::preVisit(grammar::ast::FuncDecl &func_decl) {
    // caller save registers
    code.push_back(Instruction(Op::PUSHQ, Arg(Register::RBP, DIR())));
    code.push_back(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RBP, DIR())));
    std::vector<VarSymbol*> var_decls = func_decl.sym->symTab->get_var_symbols();
    // code.push_back(Instruction(Op::SUBQ, Arg(ImmediateValue(8 * var_decls.size()), DIR()), Arg(Register::RSP, DIR())));
    for (int i = 0; i < var_decls.size(); i++) {
        code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR())));
    }
}

void IRVisitor::postVisit(grammar::ast::FuncDecl &func_decl) {
    code.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::RSP, DIR())));
    code.push_back(Instruction(Op::POPQ, Arg(Register::RBP, DIR())));
}

void IRVisitor::postVisit(grammar::ast::VarDeclAssign &var_decl_assign) {
    AstValue value = pop(temp_storage);
    if (std::holds_alternative<int>(value)) {
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(std::get<int>(value)), DIR()), Arg(GenericRegister(var_decl_assign.decl.sym->local_id), DIR())));
    } else if (std::holds_alternative<bool>(value)) {
        bool bool_value = std::get<bool>(value);
        int int_value = bool_value ? 1 : 0;
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(GenericRegister(var_decl_assign.decl.sym->local_id), DIR())));
    } else if (std::holds_alternative<GenericRegister>(value)) {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(value), DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(GenericRegister(var_decl_assign.decl.sym->local_id), DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::R8, DIR())));
    }
}

void IRVisitor::preVisit(int &i) {
    temp_storage.push(i);
}

void IRVisitor::preVisit(bool &b) {
    temp_storage.push(b);
}

void IRVisitor::postVisit(grammar::ast::VarExpression &var_expr) {
    VarSymbol *var_symbol = static_cast<VarSymbol*>(var_expr.id.sym);
    temp_storage.push(var_symbol->local_id);
}

void IRVisitor::binopInstructions(std::string op, GenericRegister result){
    if (op == "+") {
        code.push_back(Instruction(Op::ADDQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "-") {
        code.push_back(Instruction(Op::SUBQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "*") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::IMULQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::RDX, DIR())));
    } else if (op == "/") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::RAX, DIR())));
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RAX, DIR())));
        code.push_back(Instruction(Op::IDIVQ, Arg(Register::R9, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::RAX, DIR())));
    } else if (op == "%") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::RAX, DIR())));
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RAX, DIR())));
        code.push_back(Instruction(Op::IDIVQ, Arg(Register::R9, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::RDX, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::RAX, DIR())));
    } else if (op == "&") {
        code.push_back(Instruction(Op::ANDQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "|") {
        code.push_back(Instruction(Op::ORQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "<") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETL, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::R10, DIR())));
    } else if (op == ">") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETG, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::R10, DIR())));
    } else if (op == "==") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::R10, DIR())));
    } else if (op == "!=") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETNE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::R10, DIR())));
    } else if (op == "<=") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETLE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::R10, DIR())));
    } else if (op == ">=") {
        code.push_back(Instruction(Op::PUSHQ, Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETGE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        code.push_back(Instruction(Op::POPQ, Arg(Register::R10, DIR())));
    }
}


void IRVisitor::postVisit(grammar::ast::Rhs &op_exp) {
    // future optimization: calculate immediate values immediately to optimize program.
    code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR())));    
    code.push_back(Instruction(Op::PUSHQ, Arg(Register::R8, DIR())));
    code.push_back(Instruction(Op::PUSHQ, Arg(Register::R9, DIR())));


    AstValue rhs = pop(temp_storage);
    AstValue lhs = pop(temp_storage);

    if (std::holds_alternative<int>(lhs)) {
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(std::get<int>(lhs)), DIR()), Arg(Register::R8, DIR())));
    } else if (std::holds_alternative<bool>(lhs)) {
        bool bool_value = std::get<bool>(lhs);
        int int_value = bool_value ? 1 : 0;
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(Register::R8, DIR())));
    } else if (std::holds_alternative<GenericRegister>(lhs)) {
        code.push_back(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(lhs), DIR()), Arg(Register::R8, DIR())));
    }


    GenericRegister result = GenericRegister(++op_exp.scope->registerCounter);
    if (std::holds_alternative<int>(rhs)) {
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(std::get<int>(rhs)), DIR()), Arg(Register::R9, DIR())));
    } else if (std::holds_alternative<bool>(rhs)) {
        bool bool_value = std::get<bool>(rhs);
        int int_value = bool_value ? 1 : 0;
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(Register::R9, DIR())));
    } else if (std::holds_alternative<GenericRegister>(rhs)) {
        code.push_back(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(rhs), DIR()), Arg(Register::R9, DIR())));
    }
    binopInstructions(op_exp.op, result);
    
    temp_storage.push(result);  
    code.push_back(Instruction(Op::POPQ, Arg(Register::R9, DIR())));
    code.push_back(Instruction(Op::POPQ, Arg(Register::R8, DIR())));
}

void IRVisitor::postVisit(grammar::ast::PrintStatement &print) {
    AstValue value = pop(temp_storage);
    if (std::holds_alternative<int>(value)) { 
        code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(ImmediateValue(std::get<int>(value)), DIR())));
    } else if (std::holds_alternative<bool>(value)) {
        bool bool_value = std::get<bool>(value);
        int int_value = bool_value ? 1 : 0;
        code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(ImmediateValue(int_value), DIR())));
    } else if (std::holds_alternative<GenericRegister>(value)) {
        code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(GenericRegister(std::get<GenericRegister>(value).local_id), DIR())));
    }
}


template<typename T>
T IRVisitor::pop(std::stack<T>& myStack) {
    if (myStack.empty()) {
        throw std::runtime_error("Attempting to pop from an empty stack");
    }
    T topElement = std::move(myStack.top());
    myStack.pop();
    return topElement;
}

IR intermediate_code_generation(grammar::ast::Prog &prog) {
    auto visitor = IRVisitor();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return std::move(visitor.code);
}



