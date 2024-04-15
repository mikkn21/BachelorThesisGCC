#include "code_generation.hpp"
#include "../semantics/symbol_table.hpp"


#include <string>
// #include <boost/spirit/home/x3.hpp>
// #include <boost/spirit/home/x3/support/ast/apply_visitor.hpp>
#include <iostream>
const int callee_offset = -40;
const int arg_offset = 16;

std::vector<Instruction> static_link_instructions(int depth, int target_local_id, GenericRegister& result_register) {
    std::vector<Instruction> instructions;
    instructions.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value

    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, IRL(-16)), Arg(Register::R8, DIR())));

    for (auto i = 0; i < depth; i++) {
        instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, IRL(-16)), Arg(Register::R9, DIR())));
        instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
    }
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, IRL(target_local_id)), Arg(result_register, DIR())));
    return instructions;
}


IRVisitor::IRVisitor() : Visitor() {}

void IRVisitor::postVisit(grammar::ast::ReturnStatement &return_statement) {
    AstValue value = pop(temp_storage);
    if (std::holds_alternative<int>(value)) {
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(std::get<int>(value)), DIR()), Arg(Register::RAX, DIR())));
    } else if (std::holds_alternative<bool>(value)) {
        bool bool_value = std::get<bool>(value);
        int int_value = bool_value ? 1 : 0;
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(Register::RAX, DIR())));
    } else if (std::holds_alternative<GenericRegister>(value)) {
        code.push_back(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(value), DIR()), Arg(Register::RAX, DIR())));
    }
    // Remove local variables from the stack
    code.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR())));
    code.push_back(Instruction(Op::ADDQ, Arg(ImmediateValue(callee_offset), DIR()), Arg(Register::R8, DIR())));
    code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RSP, DIR())));

    code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_RESTORE, DIR())));
    code.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::RSP, DIR())));
    code.push_back(Instruction(Op::POPQ, Arg(Register::RBP, DIR())));
    code.push_back(Instruction(Op::RET));
}

void IRVisitor::preVisit(grammar::ast::FuncDecl &func_decl) {
    code.push_back(Instruction(Op::LABEL, Arg(Label(func_decl.id.id), DIR())));
    code.push_back(Instruction(Op::PUSHQ, Arg(Register::RBP, DIR())));
    code.push_back(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RBP, DIR())));
    code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_SAVE, DIR())));
    std::vector<VarSymbol*> var_decls = func_decl.sym->symTab->get_var_symbols();
    for (long unsigned int i = 0; i < var_decls.size(); i++) {
        code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR())));
    }
}

void IRVisitor::postVisit(grammar::ast::FunctionCall &func_call) {
    code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
    GenericRegister result = GenericRegister(++func_call.id.scope->registerCounter); // register for the function result to be stored in

    code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_SAVE, DIR())));

    // add arguments to stack in reverse order
    for (long unsigned int i = 0; i < func_call.argument_list.arguments.size(); i++) {
        AstValue value = pop(temp_storage);
        if (std::holds_alternative<int>(value)) {
            code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(std::get<int>(value)), DIR())));
        } else if (std::holds_alternative<bool>(value)) {
            bool bool_value = std::get<bool>(value);
            int int_value = bool_value ? 1 : 0;
            code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(int_value), DIR())));
        } else if (std::holds_alternative<GenericRegister>(value)) {
            code.push_back(Instruction(Op::PUSHQ, Arg(std::get<GenericRegister>(value), DIR())));
        }
    }
    int callee_depth = static_cast<FuncSymbol*>(func_call.id.sym)->symTab->parentScope->depth;
    int caller_depth = func_call.id.scope->depth;
    int difference = caller_depth - callee_depth;
    
    code.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR())));
    for (auto i = 0; i < difference; i++) {
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, IRL(-16)), Arg(Register::R9, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
    } 

    code.push_back(Instruction(Op::PUSHQ, Arg(Register::R8, DIR()))); // Settting static link.
    code.push_back(Instruction(Op::CALL, Arg(Label(func_call.id.id), DIR())));
    code.push_back(Instruction(Op::ADDQ, Arg(ImmediateValue((func_call.argument_list.arguments.size()+1) * 8), DIR()), Arg(Register::RSP, DIR()))); // remove arguments and static æink from stack
    code.push_back(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR()))); // save result to a save register
    code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_RESTORE, DIR())));
    temp_storage.push(result); // pushing the result of the function
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
        code.push_back(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(value), DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(GenericRegister(var_decl_assign.decl.sym->local_id), DIR())));
    } else {
        throw IRError("Unexpected type in VarDeclAssign");
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
    int k = var_expr.id.scope->depth;
    int l = 0;

    auto o = var_symbol->varDecl;
    int difference = k - l;
    GenericRegister result_register = GenericRegister(++var_expr.id.scope->registerCounter);
    auto new_code = static_link_instructions(difference, var_symbol->local_id, result_register);
    code.insert(code.end(), new_code.begin(), new_code.end());
    temp_storage.push(result_register);
}

void IRVisitor::binopInstructions(std::string op, GenericRegister result){
    if (op == "+") {
        code.push_back(Instruction(Op::ADDQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "-") {
        code.push_back(Instruction(Op::SUBQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "*") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::IMULQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "/") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RAX, DIR())));
        code.push_back(Instruction(Op::IDIVQ, Arg(Register::R9, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR())));
    } else if (op == "%") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RAX, DIR())));
        code.push_back(Instruction(Op::IDIVQ, Arg(Register::R9, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::RDX, DIR()), Arg(result, DIR())));
    } else if (op == "&") {
        code.push_back(Instruction(Op::ANDQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "|") {
        code.push_back(Instruction(Op::ORQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "<") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETL, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == ">") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETG, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "==") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "!=") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETNE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "<=") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETLE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == ">=") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETGE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    }
}


void IRVisitor::postVisit(grammar::ast::Rhs &op_exp) {
    // future optimization: calculate immediate values immediately to optimize program.
    code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register
    GenericRegister result = GenericRegister(++op_exp.scope->registerCounter);

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
        code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(std::get<GenericRegister>(value), DIR())));
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


