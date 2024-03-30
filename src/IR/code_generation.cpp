#include "code_generation.hpp"

IRVisitor::IRVisitor() : Visitor(), register_counter(0) {}

void IRVisitor::preVisit(FuncDecl &func_decl) {
    vector<VarSymbol*> var_decls = func_decl.sym->symTab->get_var_symbols();
    // cout << var_decls.size() << endl;
    for (int i = 0; i < var_decls.size(); i++) {
        code.push_back(Instruction(Op::PUSH, Arg(ImmediateValue(0), DIR())));
        var_decls[i]->local_id = i+1;
        // printf("id: %d\n", i+1);
    
    }
}

void IRVisitor::postVisit(VarDecl &var_decl) {
    AstValue value = pop(temp_storage);
    if (holds_alternative<int>(value)) {
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(get<int>(value)), DIR()), Arg(GenericRegister(var_decl.sym->local_id), DIR())));
    } else if (holds_alternative<bool>(value)) {
        bool bool_value = get<bool>(value);
        int int_value = bool_value ? 1 : 0;
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(GenericRegister(var_decl.sym->local_id), DIR())));
    }
}

void IRVisitor::preVisit(int &i) {
    temp_storage.push(i);
}
// TODO: how to access symbol table from here?
// void IRVisitor::postVisit(VarExpression &var_expr) {
//     code.push_back(Instruction(Op::MOVQ, Arg(GenericRegister(var_expr.id), DIR()), Arg(GenericRegister(new_register()), DIR()));
// }

void IRVisitor::postVisit(PrintStatement &print) {
    AstValue value = pop(temp_storage);
    if (holds_alternative<int>(value)) { 
        code.push_back(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(ImmediateValue(get<int>(value)), DIR())));
    }
}


int IRVisitor::new_register() { return register_counter++; }

template<typename T>
T IRVisitor::pop(stack<T>& myStack) {
    if (myStack.empty()) {
        throw runtime_error("Attempting to pop from an empty stack");
    }
    T topElement = move(myStack.top());
    myStack.pop();
    return topElement;
}

IR intermediate_code_generation(Prog &prog) {
    auto visitor = IRVisitor();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return move(visitor.code);
}



