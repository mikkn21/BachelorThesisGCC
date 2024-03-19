#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "emit.hpp"

string printIMM(int number) {
    string s = "leaq format(%rip), %rdi\n" 
    "movq $" + std::to_string(number) + ", %rsi\n"
    "xorq %rax, %rax\n"
    "call printf@plt";
    return s;
}

string procedure(Instruction instruction) {
    if (holds_alternative<Procedure>(instruction.args[0].target)) {
        switch (get<Procedure>(instruction.args[0].target)) {
            case Procedure::PRINT: 
                if (holds_alternative<ImmediateValue>(instruction.args[1].target)) {
                    return printIMM(get<ImmediateValue>(instruction.args[1].target).value);
                } else if (holds_alternative<Register>(instruction.args[1].target)) {
                    throw IRError("not implemented yet");
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
        outputFile << "format: .asciz \"%d\n\"\n" << endl;
        outputFile << ".globl _start" << endl;
        outputFile << "\n_start:" << endl;
        for (const Instruction& instruction : ir) {
            switch (instruction.operation) {
                case Op::PROCEDURE:
                    outputFile << procedure(instruction);
                    break;
                default: 
                    outputFile << "\t" << instruction << endl;
            }
        }
        outputFile << "movq $60, %rax" << endl;
        outputFile << "movq $0, %rdi" << endl;
        outputFile << "syscall" << endl;
        outputFile.close();
    } else {
        throw EmitError("Could not open/create output file");
    }
}

void emit(IR ir) {
    emit_to_file(move(ir));
}