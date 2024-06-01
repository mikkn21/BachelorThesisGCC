#ifndef IR_HPP
#define IR_HPP

#include <list>
#include <vector>
#include <variant>
#include <optional>
#include <iostream>
#include "../error/compiler_error.hpp"
#include <map>
#include "../semantics/symbol_table.hpp"


enum class Op {
   MOVQ, CALL, RET, CMPQ, JMP, JE, JNE, JL, JLE, JG, JGE, ADDQ, SUBQ, IMULQ, IDIVQ, LABEL, PROCEDURE, ANDQ, ORQ, XORQ, PUSHQ, POPQ, SETL, SETG, SETLE, SETGE, SETE, SETNE, SYSCALL, NOTHING, LEAQ, DUMMY
};

struct DIR {};
struct IND {};
struct IRL {
    long offset;
    IRL(long offset);
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
    GenericRegister() : local_id(0) {} // remove this maybe?
    
    bool operator<(const GenericRegister& other) const {
        return local_id < other.local_id;
    }

    bool operator==(const GenericRegister& other) const {
        return local_id == other.local_id;
    }
};

enum class Register {
    RAX, RBX, RCX, RDX, RSI, RDI, R8, R8B, R9, R10, R10B, R11, R12, R13, R14, R15, RSP, RBP, RIP
};


struct Label {
    std::string label;
    Label(const std::string& label);
};

enum class Procedure {
    CALLEE_SAVE, CALLEE_RESTORE, CALLER_SAVE, CALLER_RESTORE, PRINT, MEM_ALLOC,
};

//TODO: why is it called ImmediateData?
using TargetType = std::variant<ImmediateValue, ImmediateData, Register, GenericRegister, Label, Procedure>;

//implement equals for target type
bool operator==(const TargetType& lhs, const TargetType& rhs);

struct Arg {
    TargetType target;
    MemAccessType access_type;
    Arg(TargetType target, MemAccessType access_type);
};

struct Instruction {
    Op operation;
    std::vector<Arg> args;
    std::optional<std::string> comment;
    std::optional<Instruction*> alternative_successor = std::nullopt;

    Instruction(Op op, std::optional<std::string> comment = std::nullopt, std::optional<Instruction*> alternative = std::nullopt);
    Instruction(Op op, Arg arg1, std::optional<std::string> comment = std::nullopt, std::optional<Instruction*> alternative = std::nullopt);
    Instruction(Op op, Arg arg1, Arg arg2, std::optional<std::string> comment = std::nullopt, std::optional<Instruction*> alternative = std::nullopt);
};

class Function {
    size_t register_counter = 0;
    long stack_counter = 0;
    std::map<std::string, GenericRegister> local_var_register_map;
public:
    std::list<Instruction> code;

    Function(size_t start_register_counter, std::vector<VarSymbol*> local_vars);

    GenericRegister new_register();
    long new_stack_slot() { return stack_counter++; };
    long get_stack_counter() { return stack_counter; };

    GenericRegister get_local_var_register(VarSymbol *var_symbol);
};

class IR {
public:
    std::list<Function*> functions;
    ~IR();
};

std::ostream& operator<<(std::ostream& os, const Instruction &instruction);
std::ostream& operator<<(std::ostream& os, const Op op);
std::ostream& operator<<(std::ostream& os, const Register op);
std::ostream& operator<<(std::ostream& os, const Arg arg);
std::ostream& operator<<(std::ostream& os, const IR &ir);
std::ostream& operator<<(std::ostream& os, const Procedure procedure);
std::ostream& operator<<(std::ostream& os, const std::list<Instruction> &code);

class IRError: public CompilerError {
public: 
    using CompilerError::CompilerError;
};

#endif // IR_HPi
