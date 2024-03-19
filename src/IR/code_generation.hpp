#include "ir.hpp"

using namespace std;

using AstValue = std::variant<int, bool>;

class IRVisitor : public Visitor {
public:
    IR code;

    IRVisitor();

    void preVisit(Prog &prog) override;
    void postVisit(VarDecl &var_decl) override;
    void preVisit(int &i) override;

private:
    size_t register_counter;
    std::vector<std::string> function_container;
    std::stack<AstValue> temp_storage;

    int new_register();

    template<typename T>
    T pop(std::stack<T>& myStack);
};


IR intermediate_code_generation(Prog &prog);