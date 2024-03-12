#include "typeChecking.hpp"
#include "../visitor.hpp"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <stack>
#include <string>
#include "../ast.hpp"
#include "../semantics/symbol_table.hpp"

using namespace std;

class TypeChecker : public Visitor {


    stack<string> typeStack = stack<string>(); 



    void postVisit(Prog &prog) override {
        // Make sure the stack is empty at the end
        assert(typeStack.size() == 0);
    }

    void postVisit(StatementExpression &exp) override {
        // The type of the expression is just yeeted.
        cout << "Debug: Entering postVisit statementExpression" << endl;
        typeStack.pop();
    }

    void postVisit(VarAssign &varassign) override {
        // id 
        cout << "Debug: Entering postVisit VarAssign" << endl;
        auto t1 = pop(typeStack);
        // exp resault
        auto t2 = pop(typeStack);

        if (t1 != t2) {
             throw TypeCheckError("Variable type do not match type of evaluated expression");
        }
    }


    void postVisit(VarDecl &vardecl) override {  
        //id  
        // idea to get the type of the variable
        // auto vardeclType = vardecl.sym->type;
        // string t1;
        // if (vardeclType == 0) {
        //     t1 = "int";
        // }
        // if (vardeclType == 1 ) {
        //     t1 = "bool";
        // }
        cout << "Debug: VarDecl.sym: " << (vardecl.sym == nullptr) << endl;
        cout << "Debug: VarDecl.sym: " << vardecl.sym << endl;
        cout << "Debug: Entering postVisit VarDecl" << endl;

        auto t1 = pop(typeStack);
        // exp resault
        auto t2 = pop(typeStack);

        if (t2 != t1) {
          throw TypeCheckError("Type does not match expression");
        }
    } 

    void preBlockVisit(WhileStatement &whileStatement) override {
        // exp
        cout << "Debug: Entering preBlockVisit WhileStatement" << endl;
        auto t1 = pop(typeStack);

        if (t1 != "bool") {
            throw TypeCheckError("Expression in while statement is not a bool");
        }
    }

    void postVisit(Id &id) override {
        // Change to use the symbolTable instead
        // Push the type of the id to the stack.
        cout << "Debug: Entering postVisit Id" << endl;
    }


    void postVisit(int &value) override {
      cout << "Debug: Entering postVisit int" << endl;
      typeStack.push("int");
    }

    void postVisit(bool &value) override {
        cout << "Debug: Entering postVisit bool" << endl;
        typeStack.push("bool");
    }

    void postVisit(BinopExp &binop) override  {
        // exp resault
        cout << "Debug: Entering postVisit BinopExp" << endl;
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


Prog typeChecker(Prog &prog) {
    auto visitor = TypeChecker();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return prog;
}








