
#include <fstream>
#include "emit.hpp"


string callerSave(){
    return R"(    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %rsp
)";
}

string callerRestore(){
    return R"(    popq %rsp
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rax
)";
}



string calleeSave() {
    return R"(    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
)";
}

string calleeRestore() {
    return R"(    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
)";
}

/// @brief Allocates memory on the heap. 
/// @param size The size of the memory to allocate.
/// @return The assembly code for allocating memory.
string allocateMemory(int size) {
    string s = "\tmovq $" + std::to_string(size) + ", %rdi\n"
    "\tcall allocate\n";
    return s;
}

string printImmediateValue(int number) {
    string s = callerSave() + ""
    "\tmovq $" + std::to_string(number) + ", %rdi\n"
    "\tcall printNum\n"
    "" + callerRestore();
    return s;
}

string printStackValue(long offset) {
    string s = callerSave() + ""
    "\tmovq " + std::to_string(offset) + "(%rbp), %rdi\n"
    "\tcall printNum\n"
    "" + callerRestore();
    return s;
}

string procedure(Instruction instruction) {
    if (holds_alternative<Procedure>(instruction.args[0].target)) {
        switch (get<Procedure>(instruction.args[0].target)) {
            case Procedure::PRINT:
                if (holds_alternative<ImmediateValue>(instruction.args[1].target)) {
                    return printImmediateValue(get<ImmediateValue>(instruction.args[1].target).value);
                } else if (holds_alternative<Register>(instruction.args[1].target)) {

                    return printStackValue(get<IRL>(instruction.args[1].access_type).offset);
                } else {
                    throw IRError("Not implemented yet");
                }
            case Procedure::MEM_ALLOC:
                if (holds_alternative<ImmediateValue>(instruction.args[1].target)) {
                    return allocateMemory(get<ImmediateValue>(instruction.args[1].target).value);
                } else if (holds_alternative<Register>(instruction.args[1].target)) {
                    return allocateMemory(get<IRL>(instruction.args[1].access_type).offset);
                } else {
                    throw IRError("You done goofed.");
                }
            case Procedure::CALLER_SAVE:
                return callerSave();
            case Procedure::CALLER_RESTORE:
                return callerRestore();
            case Procedure::CALLEE_SAVE:
                return calleeSave();
            case Procedure::CALLEE_RESTORE:
                return calleeRestore();
            default:
                throw IRError("Not implemented yet");
        }
    } else {
        throw IRError("Procedure got an invalid argument in the instruction");
    }
}

void emit_to_file(IR ir) {

    ofstream outputFile("chad.s");
    if (outputFile.is_open()) {
        outputFile << ".data\n";
        outputFile << "newline: .ascii \"\\n\"\n\n";
        outputFile << ".text\n";
        outputFile << ".globl _start\n";
        outputFile << "\n_start:\n";
        //outputFile << "\tpushq %rbp\n";
        // outputFile << "\tcall main\n";
        // outputFile << "\tmovq $60, %rax\n";
        // outputFile << "\txorq %rdi, %rdi\n";
        // outputFile << "\tsyscall\n";
        for (const Instruction& instruction : ir) {
            // cout << instruction.operation << endl;
            switch (instruction.operation) {
                case Op::PROCEDURE:
                    outputFile << procedure(instruction);
                    break;
                case Op::LABEL:
                    outputFile << "\n" << instruction << ":\n";
                    break;
                case Op::NOTHING:
                    outputFile << instruction << "\n";
                    break;
                default: 
                    outputFile << "\t" << instruction << "\n";
            }
        }

        outputFile << endl;
        outputFile.close();
    } else {
        throw EmitError("Could not open/create output file");
    }
}



void emit(IR ir) {
    emit_to_file(std::move(ir));
}
