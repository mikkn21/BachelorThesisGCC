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
#include "../semantics/symbol_table.hpp"
#include <set>
#include <list>
#include <stack>
#include <iostream>

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
    long offset; // We are compiling to x86_64, so we can use 64-bit offsets
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

std::ostream& operator<<(std::ostream& os, const Instruction &instruction) {
    string args = "";
    for (auto arg : instruction.args) {
        args << arg.target;
    }
    return os //<< instruction.operation << " " << args << " " << instruction.comment;
};

std::ostream& operator<<(std::ostream& os, const Op &operation) {
            return os << instruction.op << " " << instruction.args << " " << instruction.comment;
};

using namespace std;

using AstValue = std::variant<int, bool>;
class IRVisitor : public Visitor {
    size_t register_counter = 0;
    // function container
    // a function name cannot start with an integer in front, so by giving it a unique id in front, we can garantuee the function name is unique
    std::vector<std::string> function_container;
    stack<AstValue> temp_storage = stack<AstValue>();    


    int new_register() {return register_counter++; }

public:

    IR code;

    IRVisitor() : Visitor() { }

    void preVisit(Prog &prog) {
        // add main prologue
        
        // add main epilogue
    }

    // there should be no need for preVisit for var_decl
    void postVisit(VarDecl &var_decl) override {
        // int c = (2+2) + 2
        AstValue value = pop(temp_storage);
        if (std::holds_alternative<int>(value)) {
            code.push_back(Instruction(Op::MOV, Arg(ImmediateValue(get<int>(value)), DIR()), Arg(Register(var_decl.sym->uid), DIR())));
        } else if (std::holds_alternative<bool>(value)) {
            bool bool_value = std::get<bool>(value);
            int int_value = bool_value ? 1 : 0; // convert to int for assembly
            code.push_back(Instruction(Op::MOV, Arg(ImmediateValue(int_value), DIR()), Arg(Register(var_decl.sym->uid), DIR())));
        }
    }

    void preVisit(int i) {
        temp_storage.push(i);
    }

private:

    template<typename T>
    T pop(stack<T>& myStack) {
        if (myStack.empty()) {
            throw std::runtime_error("Attempting to pop from an empty stack");
        }
        T topElement = std::move(myStack.top()); 
        myStack.pop();
        return topElement;
    } 
};

using IR = std::vector<Instruction>;

IR intermediate_code_generation(Prog &prog) {
    auto visitor = IRVisitor();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return std::move(visitor.code); 
}


Instruction addInstruction = Instruction(Op::ADD, Arg(ImmediateValue(5), IND()), Arg(SpecialRegister::RAX, DIR()));
// Instruction jmpInstruction = Instruction(Op::JMP, Arg(Label("ds"), MemAccessType::DIR));
// Instruction addInstruction = Instruction(Op::ADD, Arg(Register(5), M::IND), Arg(SpecialRegister::RAX, MemAccessType::DIR));
