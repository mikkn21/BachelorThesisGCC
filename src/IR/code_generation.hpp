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
    SymbolTable* global_scope;

    IRVisitor(SymbolTable* global_scope);

    void post_visit(grammar::ast::FuncDecl &func_decl) override;
    void pre_visit(grammar::ast::FuncDecl &func_decl) override;

    void post_visit(grammar::ast::VarDeclAssign &var_decl_assign) override;
    void post_visit(grammar::ast::VarAssign &varAssign) override;
    void pre_visit(int &i) override;
    void pre_visit(bool &b) override;
    void post_visit(grammar::ast::PrintStatement &print) override;
    void post_visit(grammar::ast::VarExpression &var_expr) override;
    void post_visit(grammar::ast::Rhs &op_exp) override;
    void post_visit(grammar::ast::FunctionCall &func_call) override;
    void post_visit(grammar::ast::ReturnStatement &return_statement) override;
    void pre_visit(grammar::ast::IfStatement &if_statement) override;
    void pre_block_visit(grammar::ast::IfStatement &if_statement) override;
    void post_visit(grammar::ast::IfStatement &if_statement) override;
    void pre_visit(grammar::ast::ElseStatement &else_statement) override;
    void post_visit(grammar::ast::ConditionalStatement &conditional_statement) override;
    void post_visit(grammar::ast::Prog &prog) override;
    void pre_visit(grammar::ast::Prog &prog) override;
    void pre_visit(grammar::ast::WhileStatement &while_statement) override;
    void post_visit(grammar::ast::WhileStatement &while_statement) override; 
    void pre_block_visit(grammar::ast::WhileStatement &while_statement) override; 
    void post_visit(grammar::ast::BreakStatement &breakStatement) override;
    void post_visit(grammar::ast::ContinueStatement &continueStatement) override;

    void pre_visit(grammar::ast::ArrayExp &arr) override; 
    void post_visit(grammar::ast::ArrayExp &arr) override;

private:
    std::vector<std::string> function_container;
    std::stack<AstValue> temp_storage;

    void pushPrintFunction();
    void pushMemAllocFunction();
    void pushStandardFunctions();

    int new_register();
    //void binopInstructions(std::string op, GenericRegister result);

    template<typename T>
    T pop(std::stack<T>& myStack);
};


IR intermediate_code_generation(grammar::ast::Prog &prog, SymbolTable* global_scope);
