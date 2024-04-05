#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "emit.hpp"

size_t unique_label_id = 0;

string get_label(string name) {
    return "L" + std::to_string(unique_label_id++) + "_" + name;
}

string callerSave(){
    return R"(pushq %rax
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
    return R"(popq %rsp
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
    return R"(pushq %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    )";
}

string calleeRestore() {
    return R"(popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp
    )";
}

string print_immediate_value(int number) {
    string s = callerSave() + ""
    "\tmovq $" + std::to_string(number) + ", %rdi\n"
    "\tcall printNum\n"
    "" + callerRestore();
    return s;
}

string print_stack_value(long offset) {
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
                    return print_immediate_value(get<ImmediateValue>(instruction.args[1].target).value);
                } else if (holds_alternative<Register>(instruction.args[1].target)) {

                    return print_stack_value(get<IRL>(instruction.args[1].access_type).offset);
                }
        }
    } else {
        throw IRError("Procedure got an invalid argument in the instruction");
    }
}

string print_function() {
    return R"(printNum:
	movq %rdi, %rax # The number

	movq $0, %r9 # Counter for chars to write
	# Convert the number to chars
.LprintNum_convertLoop:
	movq $0, %rdx
	movq $10, %rcx
	idivq %rcx
	addq $48, %rdx # '0' is 48
	pushq %rdx
	addq $1, %r9
	cmpq $0, %rax   
	jne .LprintNum_convertLoop
	# Print the number to stdout
.LprintNum_printLoop:
	movq $1, %rax # sys_write
	movq $1, %rdi # stdout
	movq %rsp, %rsi # buf
	movq $1, %rdx # len
	syscall
	addq $8, %rsp
	addq $-1, %r9
	jne .LprintNum_printLoop
.print_newline:
    movq $1, %rax
    movq $1, %rdi
    movq $newline, %rsi
    movq $1, %rdx
    syscall
    ret

)";
}


void emit_to_file(IR ir) {

    ofstream outputFile("chad.s");
    if (outputFile.is_open()) {
        outputFile << ".data\n";
        outputFile << "newline: .ascii \"\\n\"\n\n";
        outputFile << ".text\n";
        outputFile << ".globl main\n";
        outputFile << "\nmain:\n";
        for (const Instruction& instruction : ir) {
            // cout << instruction.operation << endl;
            switch (instruction.operation) {
                case Op::PROCEDURE:
                    outputFile << procedure(instruction);
                    break;
                case Op::LABEL:
                    outputFile << instruction << "\n";
                    break;
                default: 
                    outputFile << "\t" << instruction << "\n";
            }
        }
        outputFile << "\tmovq $60, %rax\n";
        outputFile << "\txorq %rdi, %rdi\n";
        outputFile << "\tsyscall\n";
        outputFile << "\n";
        outputFile << print_function();
        outputFile << endl;
        outputFile.close();
    } else {
        throw EmitError("Could not open/create output file");
    }
}



void emit(IR ir) {
    emit_to_file(move(ir));
}