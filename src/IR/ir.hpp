#ifndef IR_HPP
#define IR_HPP

#include <vector>
#include <variant>
#include <optional>
#include <iostream>
#include "../error/compiler_error.hpp"


enum class Op {
   MOVQ, PUSH, POP, CALL, RET, CMPQ, JMP, JE, JNE, JL, JLE, JG, JGE, ADDQ, SUBQ, MULQ, DIVQ, LABEL, PROCEDURE,
};

struct DIR {};
struct IND {};
struct IRL {
    long offset;
    IRL(long offset);
};

using MemAccessType = std::variant<DIR, IND, IRL>;

struct ImmediateValue {
    int value;
    ImmediateValue(int v);
};

struct GenericRegister {
    size_t id;
    GenericRegister(size_t i);
};

enum class Register {
    RBP, RSP, RAX, RSL,
};

struct Label {
    std::string label;
    Label(const std::string& l);
};

enum class Procedure {
    CALLEE_SAVE, CALLEE_RESTORE, CALLER_SAVE, CALLER_RESTORE, PRINT,
};

using TargetType = std::variant<ImmediateValue, Register, GenericRegister, Label, Procedure>;

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