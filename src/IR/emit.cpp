
#include <fstream>
#include <string>
#include "emit.hpp"


string caller_save(){
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

string caller_restore(){
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



string callee_save() {
    return R"(    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
)";
}

string callee_restore() {
    return R"(    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
)";
}

/// @brief Allocates memory on the heap using an immediate value.
/// @param size The size of the memory to allocate.
/// @return The assembly code for allocating memory.

string allocate_memory_immediate_value(int size) {
    string s = ""
    "\tmovq $" + std::to_string(size) + ", %rdi\n"
    "\tcall allocate\n";
    return s;
}

/// @brief Allocates memory on the heap using a stack value.
/// @param offset The offset of the memory to allocate.
/// @return The assembly code for allocating memory.
string allocate_memory_stack_value(long offset) {
    string s = ""
    "\tmovq " + std::to_string(offset) + "(%rbp), %rdi\n"
    "\tcall allocate\n";
    return s;
}

/// @brief Allocates memory on the heap using a stack value.
/// @param offset The offset of the memory to allocate.
/// @return The assembly code for allocating memory.
string allocate_memory_register_value(Register reg) {
    std::stringstream st;
    st << "\tmovq " << reg << ", %rdi\n"
    << "\tcall allocate\n";
    return st.str();
}

string print_immediate_value(int number) {
    string s = caller_save() + ""
    "\tmovq $" + std::to_string(number) + ", %rdi\n"
    "\tcall printNum\n"
    "" + caller_restore();
    return s;
}

string print_stack_value(long offset) {
    std::cout << "in print_stack_value\n" << std::endl;
    string s = caller_save() + ""
    "\tmovq " + std::to_string(offset) + "(%rbp), %rdi\n"
    "\tcall printNum\n"
    "" + caller_restore();
    return s;
}

std::string print_register_value(Register reg) {
    std::stringstream ss;
    ss << reg; 
    std::string assemblyCode = caller_save() +
        "\tmovq " + ss.str() + ", %rdi\n"
        "\tcall printNum\n" +
        caller_restore();
    return assemblyCode;
}
string procedure(Instruction instruction) {
    if (holds_alternative<Procedure>(instruction.args[0].target)) {
        switch (get<Procedure>(instruction.args[0].target)) {
            case Procedure::PRINT:
                if (holds_alternative<ImmediateValue>(instruction.args[1].target)) {
                    return print_immediate_value(get<ImmediateValue>(instruction.args[1].target).value);
                } else if (holds_alternative<Register>(instruction.args[1].target)) {
                    if (holds_alternative<DIR>(instruction.args[1].access_type)) {
                        return print_register_value(get<Register>(instruction.args[1].target));
                    } else  if (holds_alternative<IRL>(instruction.args[1].access_type)) {
                        return print_stack_value(get<IRL>(instruction.args[1].access_type).offset);
                    } else {
                        throw IRError("Not implemented yet");
                    }
                } else {
                    throw IRError("Not implemented yet");
                }
            case Procedure::MEM_ALLOC:
                if (holds_alternative<ImmediateValue>(instruction.args[1].target)) {
                    return allocate_memory_immediate_value(get<ImmediateValue>(instruction.args[1].target).value);
                } else if (holds_alternative<Register>(instruction.args[1].target)) {
                    return allocate_memory_register_value(get<Register>(instruction.args[1].target));
                } else {
                    throw IRError("You done goofed.");
                }
            case Procedure::CALLER_SAVE:
                return caller_save();
            case Procedure::CALLER_RESTORE:
                return caller_restore();
            case Procedure::CALLEE_SAVE:
                return callee_save();
            case Procedure::CALLEE_RESTORE:
                return callee_restore();
            default:
                throw IRError("Not implemented yet");
        }
    } else {
        throw IRError("Procedure got an invalid argument in the instruction");
    }
}

void emit_to_file(IR &ir) {

    ofstream output_file("chad.s");
    if (output_file.is_open()) {
        output_file << ".data\n";
        output_file << "true: .ascii \"true\\n\"\n";
        output_file << "false: .ascii \"false\\n\"\n";
        output_file << "beta: .ascii \"beta\\n\"\n";
        output_file << "object: .ascii \"object\\n\"\n";
        output_file << "array: .ascii \"array\\n\"\n";
        output_file << "is_beta: .ascii \"Attempted to access a beta value on line \\n\"\n";
        output_file << "newline: .ascii \"\\n\"\n\n";
        output_file << ".text\n";
        output_file << ".globl _start\n";
        // output_file << "\n_start:\n";
        //output_file << "\tpushq %rbp\n";
        // output_file << "\tcall main\n";
        // output_file << "\tmovq $60, %rax\n";
        // output_file << "\txorq %rdi, %rdi\n";
        // output_file << "\tsyscall\n";
        for (const auto func : ir.functions) {
            for (const Instruction& instruction : (*func).code) {
                switch (instruction.operation) {
                    case Op::PROCEDURE:
                        output_file << procedure(instruction);
                        break;
                    case Op::LABEL:
                        output_file << "\n" << instruction << ":\n";
                        break;
                    case Op::NOTHING:
                        output_file << instruction << "\n";
                        break;
                    default: 
                        output_file << "\t" << instruction << "\n";
                }
            }
        }

        output_file << endl;
        output_file.close();
    } else {
        throw EmitError("Could not open/create output file");
    }
}



void emit(IR &ir) {
    emit_to_file(ir);
}
