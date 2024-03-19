#include "code_generation.hpp"

IRVisitor::IRVisitor() : Visitor(), register_counter(0) {}

void IRVisitor::preVisit(Prog &prog) {
    // add main prologue
    // add main epilogue
}

void IRVisitor::postVisit(VarDecl &var_decl) {
    AstValue value = pop(temp_storage);
    if (holds_alternative<int>(value)) {
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(get<int>(value)), DIR()), Arg(GenericRegister(var_decl.sym->uid), DIR()), "int variable"));
    } else if (holds_alternative<bool>(value)) {
        bool bool_value = get<bool>(value);
        int int_value = bool_value ? 1 : 0;
        code.push_back(Instruction(Op::MOVQ, Arg(ImmediateValue(int_value), DIR()), Arg(GenericRegister(var_decl.sym->uid), DIR())));
    }
}

void IRVisitor::preVisit(int &i) {
    temp_storage.push(i);
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



