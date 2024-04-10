#include "ir.hpp"

using namespace std;

using AstValue = std::variant<int, bool, GenericRegister>;

class IRVisitor : public Visitor {
public:
    IR code;

    IRVisitor();

    void preVisit(FuncDecl &func_decl) override;
    void postVisit(FuncDecl &func_decl) override;

    void postVisit(VarDeclAssign &var_decl_assign) override;
    void preVisit(int &i) override;
    void preVisit(bool &b) override;
    void postVisit(PrintStatement &print) override;
    void postVisit(VarExpression &var_expr) override;
    void postVisit(Rhs &op_exp) override;
    void postVisit(FunctionCall &func_call) override;

private:
    std::vector<std::string> function_container;
    std::stack<AstValue> temp_storage;

    int new_register();
    void binopInstructions(string op, GenericRegister result);

    template<typename T>
    T pop(std::stack<T>& myStack);
};


IR intermediate_code_generation(Prog &prog);