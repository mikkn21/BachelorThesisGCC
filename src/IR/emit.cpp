#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "emit.hpp"

string print_immediate_value(int number) {
    string s = "\tleaq format(%rip), %rdi\n" 
    "\tmovq $" + std::to_string(number) + ", %rsi\n"
    "\txorq %rax, %rax\n"
    "\tcall printf\n";
    return s;
}

string print_stack_value(long offset) {
    string s = "\tleaq format(%rip), %rdi\n" 
    "\tmovq " + std::to_string(offset) + "(%rbp), %rsi\n"
    "\txorq %rax, %rax\n"
    "\tcall printf\n";
    return s;
}

string procedure(Instruction instruction) {
    if (holds_alternative<Procedure>(instruction.args[0].target)) {
        switch (get<Procedure>(instruction.args[0].target)) {
            case Procedure::PRINT:
                if (holds_alternative<ImmediateValue>(instruction.args[1].target)) {
                    return print_immediate_value(get<ImmediateValue>(instruction.args[1].target).value);
                } else if (holds_alternative<Register>(instruction.args[1].target)) {

                    return print_stack_value(get<IRL>(instruction.args[1].access_type).offset);
                }
        }
    } else {
        throw IRError("Procedure got an invalid argument in the instruction");
    }
}

void emit_to_file(IR ir) {

    ofstream outputFile("chad.s");
    if (outputFile.is_open()) {
        outputFile << ".data" << endl;
        outputFile << "format: .asciz \"%d\\n\"\n" << endl;
        outputFile << ".text" << endl;
        outputFile << ".globl main" << endl;
        outputFile << "\nmain:" << endl;
        for (const Instruction& instruction : ir) {
            // cout << instruction.operation << endl;
            switch (instruction.operation) {
                case Op::PROCEDURE:
                    outputFile << procedure(instruction);
                    break;
                default: 
                    outputFile << "\t" << instruction << endl;
            }
        }
        outputFile << "\tmovq $60, %rax" << endl;
        outputFile << "\tmovq $0, %rdi" << endl;
        outputFile << "\tsyscall" << endl;
        outputFile.close();
    } else {
        throw EmitError("Could not open/create output file");
    }
}

void emit(IR ir) {
    emit_to_file(move(ir));
}