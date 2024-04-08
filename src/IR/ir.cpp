#include "ir.hpp"


IRL::IRL(long offset) : offset(offset) {}
ImmediateValue::ImmediateValue(int v) : value(v) {}
GenericRegister::GenericRegister(size_t i) : id(i) {}
Label::Label(const std::string& l) : label(l) {}
Arg::Arg(TargetType target, MemAccessType access_type) : target(target), access_type(access_type) {}

Instruction::Instruction(Op op, std::optional<std::string> comment)
    : operation(op), comment(comment) {}

Instruction::Instruction(Op op, Arg arg1, std::optional<std::string> comment)
    : operation(op), comment(comment) { args.reserve(1); args.push_back(arg1); }

Instruction::Instruction(Op op, Arg arg1, Arg arg2, std::optional<std::string> comment)
    : operation(op), comment(comment) { args.reserve(2); args.push_back(arg1); args.push_back(arg2); }

std::ostream& operator<<(std::ostream& os, const Arg arg) {
    if (holds_alternative<ImmediateValue>(arg.target)) {
        os << "$" << std::get<ImmediateValue>(arg.target).value;
    } else if (holds_alternative<Register>(arg.target)) {
        if (holds_alternative<IND>(arg.access_type)) {
            os << "(" << std::get<Register>(arg.target) << ")";
        } else if (holds_alternative<IRL>(arg.access_type)) {
            os << std::get<IRL>(arg.access_type).offset << "(" << std::get<Register>(arg.target) << ")";
        } else if (holds_alternative<DIR>(arg.access_type)) {
            os << std::get<Register>(arg.target);
        } else {
            throw IRError("Unexpected access_type");
        }
    } else if (holds_alternative<GenericRegister>(arg.target)) {
        os << "Generic Register(" << std::get<GenericRegister>(arg.target).id << ")";
    } else if (holds_alternative<Label>(arg.target)) {
        os << "Label: " << std::get<Label>(arg.target).label;
    } else if (holds_alternative<Procedure>(arg.target)) {
        os << "Procedure" << std::get<Procedure>(arg.target);
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const Instruction &instruction) {
    os << instruction.operation;
    if (instruction.args.size() == 1) {
        os << " " << instruction.args[0];
    } else if (instruction.args.size() == 2) {
        os << " " << instruction.args[0] << ", " << instruction.args[1];
    }
    return os << (instruction.comment ? "\t# " + instruction.comment.value() : "");
}

std::ostream& operator<<(std::ostream& os, const Op op) {
    switch (op) {
        case Op::MOVQ:          os << "movq";       break;
        case Op::PUSH:          os << "push";       break;
        case Op::POP:           os << "pop";        break;
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
        case Op::MULQ:          os << "mulq";       break;
        case Op::DIVQ:          os << "divq";       break;
        case Op::LABEL:         os << "label";      break;
        case Op::PROCEDURE:     os << "procedure";  break;
        default:                os << "Unknown";    break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Register sp) {
    switch (sp) {
        case Register::RAX:      os << "%rax";       break;
        case Register::RBP:      os << "%rbp";       break;
        case Register::RSP:      os << "%rsp";       break;
        case Register::RSL:      os << "%rsl";       break;
        default:                        os << "Unknown";    break;
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
        default:                            os << "Unknown";            break;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const IR ir) {
    for (auto instruction : ir) {
        os << instruction << std::endl;
    }
    return os;
}

