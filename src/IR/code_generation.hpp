#include "ir.hpp"
#include "../visitor.hpp"
#include <stack>


using AstValue = std::variant<int, bool, GenericRegister>;

class IRVisitor : public Visitor {
public:
    IR code;

    IRVisitor();

    void preVisit(grammar::ast::FuncDecl &func_decl) override;
    void postVisit(grammar::ast::VarDeclAssign &var_decl_assign) override;
    void preVisit(int &i) override;
    void preVisit(bool &b) override;
    void postVisit(grammar::ast::PrintStatement &print) override;
    void postVisit(grammar::ast::VarExpression &var_expr) override;
    void postVisit(grammar::ast::Rhs &op_exp) override;
    void postVisit(grammar::ast::FunctionCall &func_call) override;
    void postVisit(grammar::ast::ReturnStatement &return_statement) override;
    void preBlockVisit(grammar::ast::IfStatement &if_statement) override;
    void preVisit(grammar::ast::ElseStatement &else_statement) override;
    void postVisit(grammar::ast::ConditionalStatement &conditional_statement) override;






private:
    std::vector<std::string> function_container;
    std::stack<AstValue> temp_storage;

    int new_register();
    void binopInstructions(std::string op, GenericRegister result);

    template<typename T>
    T pop(std::stack<T>& myStack);
};


IR intermediate_code_generation(grammar::ast::Prog &prog);