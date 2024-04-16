#include "code_generation.hpp"
#include "../semantics/symbol_table.hpp"

#include <string>
// #include <boost/spirit/home/x3.hpp>
// #include <boost/spirit/home/x3/support/ast/apply_visitor.hpp>
#include <iostream>
const int callee_offset = -40;
const int arg_offset = 16;

/// Expects there to be space on the stack for the result register taken as input. 
std::vector<Instruction> static_link_instructions(int depth, int target_local_id, GenericRegister& result_register) {
    std::vector<Instruction> instructions;
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR())));
    // std::cout << "depth: " << depth << std::endl;
    for (auto i = 0; i < depth; i++) {
        instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, IRL(16)), Arg(Register::R9, DIR())));
        instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
    }
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, IRL(target_local_id*(-8)+callee_offset)), Arg(Register::R9, DIR())));
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(result_register, DIR())));
    return instructions;
}


IRVisitor::IRVisitor(SymbolTable* globalScope) : globalScope(globalScope), Visitor() {}

void IRVisitor::postVisit(grammar::ast::ReturnStatement &return_statement) {
    AstValue value = pop(temp_storage);
    if (std::holds_alternative<int>(value)) {
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(std::get<int>(value)), DIR()), Arg(Register::RAX, DIR())));
    } else if (std::holds_alternative<bool>(value)) {
        bool bool_value = std::get<bool>(value);
        int int_value = bool_value ? 1 : 0;
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(Register::RAX, DIR())));
    } else if (std::holds_alternative<GenericRegister>(value)) {
        code.push(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(value), DIR()), Arg(Register::RAX, DIR())));
    }
    // Remove local variables from the stack
    code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR())));
    code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(callee_offset), DIR()), Arg(Register::R8, DIR())));
    code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RSP, DIR())));

    code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_RESTORE, DIR())));
    code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::RSP, DIR())));
    code.push(Instruction(Op::POPQ, Arg(Register::RBP, DIR())));
    code.push(Instruction(Op::RET));
}

void IRVisitor::preVisit(grammar::ast::FuncDecl &func_decl) {
    code.new_scope();
    code.push(Instruction(Op::LABEL, Arg(Label(func_decl.label), DIR())));
    // std::cout << func_decl.label << std::endl;
    code.push(Instruction(Op::PUSHQ, Arg(Register::RBP, DIR())));
    code.push(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RBP, DIR())));
    code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_SAVE, DIR())));
    std::vector<VarSymbol*> var_decls = func_decl.sym->symTab->get_var_symbols();
    for (long unsigned int i = 0; i < var_decls.size(); i++) {
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR())));
    }
}

void IRVisitor::postVisit(grammar::ast::FuncDecl &func_decl) {
    code.end_scope();
}

void IRVisitor::postVisit(grammar::ast::FunctionCall &func_call) {
    code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
    GenericRegister result = GenericRegister(++func_call.id.scope->registerCounter); // register for the function result to be stored in

    code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_SAVE, DIR())));

    // add arguments to stack in reverse order
    for (long unsigned int i = 0; i < func_call.argument_list.arguments.size(); i++) {
        AstValue value = pop(temp_storage);
        if (std::holds_alternative<int>(value)) {
            code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(std::get<int>(value)), DIR())));
        } else if (std::holds_alternative<bool>(value)) {
            bool bool_value = std::get<bool>(value);
            int int_value = bool_value ? 1 : 0;
            code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(int_value), DIR())));
        } else if (std::holds_alternative<GenericRegister>(value)) {
            code.push(Instruction(Op::PUSHQ, Arg(std::get<GenericRegister>(value), DIR())));
        }
    }
    int callee_depth = dynamic_cast<FuncSymbol*>(func_call.id.sym)->symTab->parentScope->depth;
    int caller_depth = func_call.id.scope->depth;
    int difference = caller_depth - callee_depth;
    
    code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR())));
    for (auto i = 0; i < difference; i++) {
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, IRL(-16)), Arg(Register::R9, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
    } 

    code.push(Instruction(Op::PUSHQ, Arg(Register::R8, DIR()))); // Settting static link.
    std::string label = dynamic_cast<FuncSymbol*>(func_call.id.sym)->funcDecl->label;
    // std::cout << label << std::endl;
    code.push(Instruction(Op::CALL, Arg(Label(label), DIR())));
    code.push(Instruction(Op::ADDQ, Arg(ImmediateValue((func_call.argument_list.arguments.size()+1) * 8), DIR()), Arg(Register::RSP, DIR()))); // remove arguments and static æink from stack
    code.push(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR()))); // save result to a save register
    code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_RESTORE, DIR())));
    temp_storage.push(result); // pushing the result of the function
}

void IRVisitor::postVisit(grammar::ast::VarDeclAssign &var_decl_assign) {
    AstValue value = pop(temp_storage);
    if (std::holds_alternative<int>(value)) {
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(std::get<int>(value)), DIR()), Arg(GenericRegister(var_decl_assign.decl.sym->local_id), DIR())));
    } else if (std::holds_alternative<bool>(value)) {
        bool bool_value = std::get<bool>(value);
        int int_value = bool_value ? 1 : 0;
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(GenericRegister(var_decl_assign.decl.sym->local_id), DIR())));
    } else if (std::holds_alternative<GenericRegister>(value)) {
        code.push(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(value), DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(GenericRegister(var_decl_assign.decl.sym->local_id), DIR())));
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
    VarSymbol *var_symbol = dynamic_cast<VarSymbol*>(var_expr.idAccess.ids.back().sym);
    auto target_depth = var_symbol->varDecl->id.scope->depth;

    int current_depth = var_expr.idAccess.ids.back().scope->depth;


    int difference = current_depth - target_depth;
    // std::cout << "current depth: " << current_depth << "target depth: " << target_depth << "difference: " << difference << std::endl;
    code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
    GenericRegister result_register = GenericRegister(++var_expr.idAccess.ids.back().scope->registerCounter);
    auto static_linking_code = static_link_instructions(difference, var_symbol->local_id, result_register);
    for (auto instruction : static_linking_code) {
        code.push(instruction);
    }
    temp_storage.push(result_register);
}

void IRVisitor::binopInstructions(std::string op, GenericRegister result){
    if (op == "+") {
        code.push(Instruction(Op::ADDQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "-") {
        code.push(Instruction(Op::SUBQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "*") {
        code.push(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push(Instruction(Op::IMULQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "/") {
        code.push(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RAX, DIR())));
        code.push(Instruction(Op::IDIVQ, Arg(Register::R9, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR())));
    } else if (op == "%") {
        code.push(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RAX, DIR())));
        code.push(Instruction(Op::IDIVQ, Arg(Register::R9, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::RDX, DIR()), Arg(result, DIR())));
    } else if (op == "&") {
        code.push(Instruction(Op::ANDQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "|") {
        code.push(Instruction(Op::ORQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "<") {
        code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::SETL, Arg(Register::R10B, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == ">") {
        code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::SETG, Arg(Register::R10B, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "==") {
        code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::SETE, Arg(Register::R10B, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "!=") {
        code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::SETNE, Arg(Register::R10B, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "<=") {
        code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::SETLE, Arg(Register::R10B, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == ">=") {
        code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::SETGE, Arg(Register::R10B, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    }
}


void IRVisitor::postVisit(grammar::ast::Rhs &op_exp) {
    // future optimization: calculate immediate values immediately to optimize program.
    code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register
    GenericRegister result = GenericRegister(++op_exp.scope->registerCounter);

    AstValue rhs = pop(temp_storage);
    AstValue lhs = pop(temp_storage);

    if (std::holds_alternative<int>(lhs)) {
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(std::get<int>(lhs)), DIR()), Arg(Register::R8, DIR())));
    } else if (std::holds_alternative<bool>(lhs)) {
        bool bool_value = std::get<bool>(lhs);
        int int_value = bool_value ? 1 : 0;
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(Register::R8, DIR())));
    } else if (std::holds_alternative<GenericRegister>(lhs)) {
        code.push(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(lhs), DIR()), Arg(Register::R8, DIR())));
    }

    if (std::holds_alternative<int>(rhs)) {
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(std::get<int>(rhs)), DIR()), Arg(Register::R9, DIR())));
    } else if (std::holds_alternative<bool>(rhs)) {
        bool bool_value = std::get<bool>(rhs);
        int int_value = bool_value ? 1 : 0;
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(Register::R9, DIR())));
    } else if (std::holds_alternative<GenericRegister>(rhs)) {
        code.push(Instruction(Op::MOVQ, Arg(std::get<GenericRegister>(rhs), DIR()), Arg(Register::R9, DIR())));
    }
    binopInstructions(op_exp.op, result);
    
    temp_storage.push(result);  
}

void IRVisitor::postVisit(grammar::ast::PrintStatement &print) {
    AstValue value = pop(temp_storage);
    if (std::holds_alternative<int>(value)) { 
        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(ImmediateValue(std::get<int>(value)), DIR())));
    } else if (std::holds_alternative<bool>(value)) {
        bool bool_value = std::get<bool>(value);
        int int_value = bool_value ? 1 : 0;
        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(ImmediateValue(int_value), DIR())));
    } else if (std::holds_alternative<GenericRegister>(value)) {
        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(std::get<GenericRegister>(value), DIR())));
    }
}



void IRVisitor::preBlockVisit(grammar::ast::IfStatement &if_statement) {
    code.push(Instruction(Op::LABEL, Arg(Label(if_statement.label), DIR())));
    AstValue expr = pop(temp_storage);
    if (std::holds_alternative<bool>(expr)) {
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(1), DIR()), Arg(ImmediateValue(std::get<bool>(expr)), DIR())));        
    } else if (std::holds_alternative<GenericRegister>(expr)) {
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(1), DIR()), Arg(std::get<GenericRegister>(expr), DIR())));
    } else {
        throw IRError("Unexpected type in ConditionalStatement");
    }

    code.push(Instruction(Op::JNE, Arg(Label(if_statement.nextLabel), DIR())));
}

void IRVisitor::postVisit(grammar::ast::IfStatement &if_statement) {
    code.push(Instruction(Op::JMP, Arg(Label(if_statement.endIfLabel), DIR())));
}

void IRVisitor::preVisit(grammar::ast::ElseStatement &else_statement) {
    code.push(Instruction(Op::LABEL, Arg(Label(else_statement.label), DIR())));
}

void IRVisitor::postVisit(grammar::ast::ConditionalStatement &condStatement) {
    code.push(Instruction(Op::LABEL, Arg(Label(condStatement.endIfLabel), DIR())));
}


void IRVisitor::preVisit(grammar::ast::Prog &prog) {
    code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_SAVE, DIR())));
    // std::cout << globalScope->get_var_symbols().size() << std::endl;
    for (auto var : globalScope->get_var_symbols()) {
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR())));
    }
}

void IRVisitor::postVisit(grammar::ast::Prog &prog) {
    code.push(Instruction(Op::CALL, Arg(Label("main"), DIR())));
    code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(60), DIR()), Arg(Register::RAX, DIR())));
    code.push(Instruction(Op::XORQ, Arg(Register::RDI, DIR()), Arg(Register::RDI, DIR())));
    code.push(Instruction(Op::SYSCALL));
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

IR intermediate_code_generation(grammar::ast::Prog &prog, SymbolTable *globalScope) {
    
    auto visitor = IRVisitor(globalScope);
    visitor.code.new_scope(); // add global scope
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    visitor.code.end_scope(); // end global scope
    return std::move(visitor.code.get_instructions());
}


FunctionOrderManager::FunctionOrderManager() : current_function_index(), list_of_funcs() {}

/// adds a new list of instructions to the current scope
void FunctionOrderManager::new_scope() {
    list_of_funcs.push_back(std::vector<Instruction>());
    current_function_index.push(list_of_funcs.size()-1);
}

/// pushes an instruction to the current scope
void FunctionOrderManager::push(Instruction instruction) {
    // auto l = current_function_index.top();
    // std::cout << "l: " << l << std::endl;
    // std::cout << "func: " << list_of_funcs[l] << std::endl;
    list_of_funcs[current_function_index.top()].push_back(instruction);
}

/// pops the current scope
void FunctionOrderManager::end_scope() {
    current_function_index.pop();
}

/// get all the instructions in preorder.
IR FunctionOrderManager::get_instructions() {
    IR instructions;
    for (auto &func : list_of_funcs) {
        instructions.insert(instructions.end(), func.begin(), func.end());
    }
    return instructions;
}