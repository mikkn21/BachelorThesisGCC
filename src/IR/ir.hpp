#ifndef IR_HPP
#define IR_HPP

#include <vector>
#include <variant>
#include <optional>
#include <iostream>
#include "../error/compiler_error.hpp"


enum class Op {
   MOVQ, PUSH, POP, CALL, RET, CMPQ, JMP, JE, JNE, JL, JLE, JG, JGE, ADDQ, SUBQ, IMULQ, IDIVQ, LABEL, PROCEDURE, ANDQ, ORQ, XORQ, PUSHQ, POPQ, SETL, SETG, SETLE, SETGE, SETE, SETNE, SYSCALL, NOTHING
};

struct DIR {};
struct IND {};
struct IRL {
    size_t offset;
    IRL(size_t offset);
};

using MemAccessType = std::variant<DIR, IND, IRL>;

struct ImmediateValue {
    int value; // TODO: what size should this be?
    ImmediateValue(int value);
};

struct ImmediateData {
    std::string value;
    ImmediateData(std::string value);
};

struct GenericRegister {
    long local_id; // TODO: should this not be a long long?
    GenericRegister(long local_id);
};

enum class Register {
    RAX, RBX, RCX, RDX, RSI, RDI, R8, R8B, R9, R10, R10B, R11, R12, R13, R14, R15, RSP, RBP, 
};

struct Label {
    std::string label;
    Label(const std::string& label);
};

enum class Procedure {
    CALLEE_SAVE, CALLEE_RESTORE, CALLER_SAVE, CALLER_RESTORE, PRINT, MEM_ALLOC,
};

using TargetType = std::variant<ImmediateValue, ImmediateData, Register, GenericRegister, Label, Procedure>;

struct Arg {
    TargetType target;
    MemAccessType access_type;
    Arg(TargetType target, MemAccessType access_type);
};

struct Instruction {
    Op operation;
    std::vector<Arg> args;
    std::optional<std::string> comment;

    Instruction(Op op, std::optional<std::string> comment = std::nullopt);
    Instruction(Op op, Arg arg1, std::optional<std::string> comment = std::nullopt);
    Instruction(Op op, Arg arg1, Arg arg2, std::optional<std::string> comment = std::nullopt);
};

using IR = std::vector<Instruction>;

std::ostream& operator<<(std::ostream& os, const Instruction &instruction);
std::ostream& operator<<(std::ostream& os, const Op op);
std::ostream& operator<<(std::ostream& os, const Register op);
std::ostream& operator<<(std::ostream& os, const Arg arg);
std::ostream& operator<<(std::ostream& os, const IR ir);
std::ostream& operator<<(std::ostream& os, const Procedure procedure);

class IRError: public CompilerError {
public: 
    using CompilerError::CompilerError;
};

#endif // IR_HPP
