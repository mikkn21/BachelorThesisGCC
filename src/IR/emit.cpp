
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
    string s = callerSave() + ""
    "\tmovq $" + std::to_string(size) + ", %rdi\n"
    "\tcall allocate\n"
    "" + callerRestore();
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

string printFunction() {
    return R"(
printNum:
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

/// @brief  A naive memory allocator that simply retrieves some new space from the OS. It is not possible to deallocate the memory again.
/// @return The assembly code for the memory allocator.
string memmoryAllocatorFunction() {
    return R"(
allocate:
    push %rdi
    # 1. Find the current end of the data segment.
    movq $12, %rax          # brk
    xorq %rdi, %rdi         # 0 means we retrieve the current end.
    syscall
    # 2. Add the amount of memory we want to allocate.
    pop %rdi                # the argument
    push %rax               # current end, which is where the allocated memory will start
    addq %rax, %rdi         # compute the new end
    movq $12, %rax          # brk
    syscall
    pop %rax                # the old end, which is the address of our allocated memory
    ret

)";
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
                default: 
                    outputFile << "\t" << instruction << "\n";
            }
        }

        // outputFile << printFunction();
        outputFile << memmoryAllocatorFunction();
        outputFile << endl;
        outputFile.close();
    } else {
        throw EmitError("Could not open/create output file");
    }
}



void emit(IR ir) {
    emit_to_file(std::move(ir));
}
