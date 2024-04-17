#include "ir.hpp"
#include "../visitor.hpp"
#include <stack>


using AstValue = std::variant<int, bool, GenericRegister>;

class FunctionOrderManager {
private:
    std::stack<size_t> current_function_index;
    std::vector<std::vector<Instruction>> list_of_funcs;
public:
    FunctionOrderManager();
    void new_scope();
    void end_scope();
    void push(Instruction Instruction);
    IR get_instructions();
};


class IRVisitor : public Visitor {
public:
    FunctionOrderManager code;
    SymbolTable* globalScope;

    IRVisitor(SymbolTable* globalScope);

    void postVisit(grammar::ast::FuncDecl &func_decl) override;
    void preVisit(grammar::ast::FuncDecl &func_decl) override;

    void postVisit(grammar::ast::VarDeclAssign &var_decl_assign) override;
    void postVisit(grammar::ast::VarAssign &varAssign) override;
    void preVisit(int &i) override;
    void preVisit(bool &b) override;
    void postVisit(grammar::ast::PrintStatement &print) override;
    void postVisit(grammar::ast::VarExpression &var_expr) override;
    void postVisit(grammar::ast::Rhs &op_exp) override;
    void postVisit(grammar::ast::FunctionCall &func_call) override;
    void postVisit(grammar::ast::ReturnStatement &return_statement) override;
    void preBlockVisit(grammar::ast::IfStatement &if_statement) override;
    void postVisit(grammar::ast::IfStatement &ifStatement) override;
    void preVisit(grammar::ast::ElseStatement &else_statement) override;
    void postVisit(grammar::ast::ConditionalStatement &conditional_statement) override;
    void postVisit(grammar::ast::Prog &prog) override;
    void preVisit(grammar::ast::Prog &prog) override;
    void preVisit(grammar::ast::WhileStatement &while_statement) override;
    void postVisit(grammar::ast::WhileStatement &while_statement) override; 
    void preBlockVisit(grammar::ast::WhileStatement &while_statement) override; 
    void postVisit(grammar::ast::BreakStatement &breakStatement) override;
    void postVisit(grammar::ast::ContinueStatement &continueStatement) override;


private:
    std::vector<std::string> function_container;
    std::stack<AstValue> temp_storage;

    int new_register();
    //void binopInstructions(std::string op, GenericRegister result);

    template<typename T>
    T pop(std::stack<T>& myStack);
};


IR intermediate_code_generation(grammar::ast::Prog &prog, SymbolTable* globalScope);