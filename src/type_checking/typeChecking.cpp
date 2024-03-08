#include "typeChecking.hpp"
#include "../visitor.hpp"
#include <cassert>
#include <stack>
#include <string>

namespace grammar {

using namespace std;

class TypeChecker : Visitor {


    stack<string> typeStack = stack<string>(); 



    void postVisit(Prog &prog) override {
        // Make sure the stack is empty at the end
        assert(typeStack.size() == 0);
    }

    void postVisit(StatementExpression &exp) override {
        // The type of the expression is just yeeted.
        typeStack.pop();
    }

    void postVisit(VarAssign &varassign) override {
        // id 
        auto t1 = pop(typeStack);
        // exp resault
        auto t2 = pop(typeStack);

        if (t1 != t2) {
             throw TypeCheckError("Variable type do not match type of evaluated expression");
        }
    }


    void postVisit(VarDecl &vardecl) override {  
        // id  
        auto t1 = pop(typeStack);
        // exp resault 
        auto t2 = pop(typeStack);

        if (t2 != t1) {
            throw TypeCheckError("Type does not match expression");
        }
    } 

    void preBlockVisit(WhileStatement &whileStatement) override {
        // exp
        auto t1 = pop(typeStack);

        if (t1 != "bool") {
            throw TypeCheckError("Expression in while statement is not a bool");
        }
    }

    void postVisit(Id &id) override {
        // Change to use the symbolTable instead
        // Push the type of the id to the stack.
    }


    void postVisit(int &value) override {
        typeStack.push("int");
    }

    void postVisit(bool &value) override {
        typeStack.push("bool");
    }

    void postVisit(BinopExp &binop) override  {
        // exp resault
        auto t1 = pop(typeStack);
        // exp resault
        auto t2 = pop(typeStack);
        
        if (t2 != t1) {
            throw TypeCheckError("Type of lefthand side does not match Type of righthand side");
        }

        auto const op = binop.op;
        if (op == "==" || op == "!=" || op == "<=" || op == ">=" ||
        op == "<"  || op == ">"  || op == "&"  || op == "|") {
            typeStack.push("bool");
        } else {
            typeStack.push(t1);
        }
    }


    Prog typeChecker(Prog &prog) {
        auto visitor = TypeChecker();
        auto traveler = TreeTraveler(visitor);
        traveler(prog);
        return prog;
    }


private:

    // template<typename T>
    // void clear(stack<T>& myStack) {
    //     myStack = stack<T>(); 
    // }
   

    template<typename T>
    T pop(stack<T>& myStack) {
        T topElement = std::move(myStack.top()); 
        myStack.pop();
        return topElement;
    } 
}; 





} // namespace grammar





