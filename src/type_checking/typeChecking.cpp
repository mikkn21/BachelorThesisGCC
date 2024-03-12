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
    

    // void postVisit(PrimitiveType &primType) override {
    //     cout << "Debug: Entering postVisit PrimitiveType" << endl;

    //     if (primType.type == "int") {
    //         typeStack.push("prim_int");
    //     }
    //     else if (primType.type == "bool") {
    //         typeStack.push("prim_bool");
    //     }
    //     else {
    //         typeStack.push("unknown"); // catch all to get correct errors
    //     }

    // }

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
        auto t1 = pop(typeStack);
        cout << "Debug: postVisit VarDecl t1: " << t1 << endl;
        // exp resault
        auto t2 = pop(typeStack);
        cout << "Debug: postVisit VarDecl t2: " << t2 << endl;
        if (t2 != t1) {
            // cout << "Types do not mathch" << endl;
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
        if (auto *varSymbolPtr = std::get_if<VarSymbol*>(&id.sym)) {
            VarSymbol* varSymbol = *varSymbolPtr; 
            typeStack.push(varSymbol->type == 0 ? "int" : "bool");
          
            std::cout << "Debug: looked at: " << id.id << " Entering postVisit Id with type (VarSymbol*) " << varSymbol->type << " = " << (varSymbol->type == 0 ? "int" : "bool")
                          << std::endl;

        } else if (auto *funcSymbolPtr = std::get_if<FuncSymbol*>(&id.sym)) {
            FuncSymbol* funcSymbol = *funcSymbolPtr; 
            typeStack.push(funcSymbol->returnType == 0 ? "int" : "bool");

            std::cout << "Debug: looked at: " << id.id << " Entering postVisit Id with type (funcSymbol*) " << funcSymbol->returnType << " = " << (funcSymbol->returnType == 0 ? "int" : "bool")
                          << std::endl;

        } else {
            throw TypeCheckError("uninitialized symbol --> " + id.id);
        }
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
        if (myStack.empty()) {
            throw std::runtime_error("Attempting to pop from an empty stack");
        }
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








