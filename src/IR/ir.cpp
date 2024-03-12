#include <vector>
#include <variant>
#include <string>
#include <vector>
#include <variant>
#include <string>
#include <optional>
//#include "ir.hpp"
#include "../ast.hpp"
#include "../visitor.hpp"
#include <set>
#include <list>

enum class Op {
    MOV,
    PUSH,
    POP,
    CALL,
    RET,
    CMP,
    JMP,
    JE,
    JNE,
    JL,
    JLE,
    JG,
    JGE,
    ADD,
    SUB,
    MUL,
    DIV,
    LABEL,
    PROCEDURE,
};


struct DIR {}; // DIRECT MEMORY ACCESS
struct IND {}; // INDIRECT MEMORY ACCESS
struct IRL { // INDIRECT RELATIVE MEMORY ACCESS
    long offset;
    IRL(long offset) : offset(offset) {}
};

using MemAccessType = std::variant<DIR, IND, IRL>;

struct ImmediateValue {
public:
    int value;

    ImmediateValue(int v) : value(v) {}
};

struct Register {
public:
    size_t id;

    Register(size_t i) : id(i) {}
};

enum class SpecialRegister {
    RBP, RSP, RAX, RSL,
};

struct Label {
public:
    std::string label;

    Label(const std::string& l) : label(l) {}
};

enum class Procedure {
    CALLE_SAVE,
    CALLE_RESTORE,
    CALLER_SAVE,
    CALLER_RESTORE,
    PRINT,
};

using TargetType = std::variant<ImmediateValue, SpecialRegister, Register, Label, Procedure>;
struct Arg {
public:
    TargetType target;
    MemAccessType access_type;

    Arg(TargetType target, MemAccessType access_type) : target(target), access_type(access_type) {}
};

using M = MemAccessType;
//using Ins = std::variant<arg0,arg1,arg2>;
struct Instruction {
    Op operation;
    std::vector<Arg> args;
    std::optional<std::string> comment;

    // Constructors for convenience
    Instruction(Op op, std::optional<std::string> comment = std::nullopt)
        : operation(op), comment(comment) {}

    Instruction(Op op, Arg arg1, std::optional<std::string> comment = std::nullopt)
        : operation(op), comment(comment) { args.reserve(1); args.push_back(arg1); }

    Instruction(Op op, Arg arg1, Arg arg2, std::optional<std::string> comment = std::nullopt)
        : operation(op), comment(comment) { args.reserve(2); args.push_back(arg1); args.push_back(arg2); }
};


class IRVisitor : public Visitor {
    size_t register_counter = 0;
    // function container
    // a function name cannot start with an integer in front, so by giving it a unique id in front, we can garantuee the function name is unique
    std::vector<std::string> function_container;

    int new_register() {return register_counter++; }

public:
    IRVisitor() : Visitor() { }

    void postVisit(VarDecl &var_decl) {
        
    }

    void preVisit(Id &id) {
        // give it unique register id
        // add it to the symbol table for id
    }

    void preVisit(Expression &exp) {
            
    }
};

struct IR {
    std::vector<Instruction> instructions;
};

IR intermediate_code_generation(Prog &prog) {
    auto visitor = IRVisitor();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return IR{}; 
}


Instruction addInstruction = Instruction(Op::ADD, Arg(ImmediateValue(5), DIR()), Arg(SpecialRegister::RAX, DIR()));
// Instruction jmpInstruction = Instruction(Op::JMP, Arg(Label("ds"), MemAccessType::DIR));
// Instruction addInstruction = Instruction(Op::ADD, Arg(Register(5), M::IND), Arg(SpecialRegister::RAX, MemAccessType::DIR));
