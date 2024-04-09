#include "ir.hpp"

using namespace std;

using AstValue = std::variant<int, bool, GenericRegister>;

class IRVisitor : public Visitor {
public:
    IR code;

    IRVisitor();

    void preVisit(FuncDecl &func_decl) override;
    void postVisit(FuncDecl &func_decl) override;

    // void postVisit(VarDecl &var_decl) override;
    void preVisit(int &i) override;
    void preVisit(bool &b) override;
    void postVisit(PrintStatement &print) override;
    void postVisit(VarExpression &var_expr) override;
    // void postVisit(BinopExps &binop_exp) override;

private:
    std::vector<std::string> function_container;
    std::stack<AstValue> temp_storage;

    int new_register();
    void binOpInstructions(string op, GenericRegister result);

    template<typename T>
    T pop(std::stack<T>& myStack);
};


IR intermediate_code_generation(Prog &prog);