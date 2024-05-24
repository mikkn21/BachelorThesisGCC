#include "ir.hpp"

IRL::IRL(long offset) : offset(offset) {}
ImmediateValue::ImmediateValue(int v) : value(v) {}
ImmediateData::ImmediateData(std::string v) : value(v) {}
/// precondition: There is room on the stack for the new register
GenericRegister::GenericRegister(long i) : local_id(i) {}
Label::Label(const std::string& l) : label(l) {}
Arg::Arg(TargetType target, MemAccessType access_type) : target(target), access_type(access_type) {}

Instruction::Instruction(Op op, std::optional<std::string> comment, std::optional<Instruction*> alternative)
    : operation(op), comment(comment), alternative_successor(alternative) {}

Instruction::Instruction(Op op, Arg arg1, std::optional<std::string> comment, std::optional<Instruction*> alternative)
    : operation(op), comment(comment), alternative_successor(alternative) {
    args.reserve(1);
    args.push_back(arg1);
}

Instruction::Instruction(Op op, Arg arg1, Arg arg2, std::optional<std::string> comment, std::optional<Instruction*> alternative)
    : operation(op), comment(comment), alternative_successor(alternative) {
    args.reserve(2);
    args.push_back(arg1);
    args.push_back(arg2);
}
std::ostream& operator<<(std::ostream& os, const Arg arg) {
    if (std::holds_alternative<ImmediateValue>(arg.target)) {
        os << "$" << std::get<ImmediateValue>(arg.target).value;
    } else if (std::holds_alternative<ImmediateData>(arg.target)) {
      os << "$" << std::get<ImmediateData>(arg.target).value;
    } else if (std::holds_alternative<Register>(arg.target)) {
        if (std::holds_alternative<IND>(arg.access_type)) {
            os << "(" << std::get<Register>(arg.target) << ")";
        } else if (std::holds_alternative<IRL>(arg.access_type)) {
            os << std::get<IRL>(arg.access_type).offset << "(" << std::get<Register>(arg.target) << ")";
        } else if (std::holds_alternative<DIR>(arg.access_type)) {
            os << std::get<Register>(arg.target);
        } else {
            throw IRError("Unexpected access_type");
        }
    } else if (std::holds_alternative<GenericRegister>(arg.target)) {
        os << "Generic Register(" << std::get<GenericRegister>(arg.target).local_id << ")";
    } else if (std::holds_alternative<Label>(arg.target)) {
        os << std::get<Label>(arg.target).label;
    } else if (std::holds_alternative<Procedure>(arg.target)) {
        os << "Procedure" << std::get<Procedure>(arg.target);
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const Instruction &instruction) {
    os << instruction.operation;
    if (instruction.operation == Op::LABEL) {
        os << instruction.args[0];
    } else if (instruction.args.size() == 1) {
        os << " " << instruction.args[0];
    } else if (instruction.args.size() == 2) {
        os << " " << instruction.args[0] << ", " << instruction.args[1];
    }
    return os << (instruction.comment ? "\t# " + instruction.comment.value() : "");
}

std::ostream& operator<<(std::ostream& os, const Op op) {
    switch (op) {
        case Op::MOVQ:          os << "movq";       break;
        case Op::PUSHQ:          os << "pushq";       break;
        case Op::CALL:          os << "call";       break;
        case Op::RET:           os << "ret";        break;
        case Op::CMPQ:          os << "cmpq";       break;
        case Op::JMP:           os << "jmp";        break;
        case Op::JE:            os << "je";         break;
        case Op::JNE:           os << "jne";        break;
        case Op::JL:            os << "jl";         break;
        case Op::JLE:           os << "jle";        break;
        case Op::JG:            os << "jg";         break;
        case Op::JGE:           os << "jge";        break;
        case Op::ADDQ:          os << "addq";       break;
        case Op::SUBQ:          os << "subq";       break;
        case Op::IMULQ:          os << "imulq";       break;
        case Op::IDIVQ:          os << "idivq";       break;
        case Op::LABEL:         os << "";      break;
        case Op::PROCEDURE:     os << "procedure";  break;
        case Op::ANDQ:           os << "andq";       break;
        case Op::ORQ:           os << "orq";       break;
        case Op::XORQ:          os << "xorq";       break;
        case Op::POPQ:          os << "popq";       break;
        case Op::SETL:          os << "setl";       break;
        case Op::SETG:          os << "setg";       break;
        case Op::SETE:          os << "sete";       break;
        case Op::SETNE:         os << "setne";       break;
        case Op::SETLE:         os << "setle";       break;
        case Op::SETGE:         os << "setge";       break;
        case Op::SYSCALL:       os << "syscall";    break;
        case Op::NOTHING:                           break;
        case Op::LEAQ:          os << "leaq";       break;
        default:                throw IRError("Invalid operation");    break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Register sp) {
    switch (sp) {
        case Register::RAX:      os << "%rax";       break;
        case Register::RBX:      os << "%rbx";       break;
        case Register::RBP:      os << "%rbp";       break;
        case Register::RSP:      os << "%rsp";       break;
        case Register::RDI:      os << "%rdi";       break;
        case Register::RSI:      os << "%rsi";       break;
        case Register::RDX:      os << "%rdx";       break;
        case Register::RCX:      os << "%rcx";       break;
        case Register::R8:       os << "%r8";        break;
        case Register::R8B:      os << "%r8b";       break;
        case Register::R9:       os << "%r9";        break;
        case Register::R10:      os << "%r10";       break;
        case Register::R10B:     os << "%r10b";      break;
        case Register::R11:      os << "%r11";       break;
        case Register::R12:      os << "%r12";       break;
        case Register::R13:      os << "%r13";       break;
        case Register::R14:      os << "%r14";       break;
        case Register::R15:      os << "%r15";       break;
        case Register::RIP:      os << "%rip";       break;
        default:                 throw IRError("Invalid Register");    break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Procedure procedure){
    switch (procedure) {
        case Procedure::CALLEE_RESTORE:     os << "CALLEE_RESTORE";     break;
        case Procedure::CALLEE_SAVE:        os << "CALLEE_SAVE";        break;
        case Procedure::CALLER_RESTORE:     os << "CALLER_RESTORE";     break;
        case Procedure::CALLER_SAVE:        os << "CALLER_SAVE";        break;
        case Procedure::PRINT:              os << "PRINT";              break;
        default:                            throw IRError("Invalid Procedure");            break;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const IR ir) {
    for (auto instruction : ir) {
        os << instruction << std::endl;
    }
    return os;
}

