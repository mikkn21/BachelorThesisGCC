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

    // the current function we are inside of
    FuncSymbol* func = nullptr;
    bool hasFuncReturned = false;

    // The stack of types
    stack<string> typeStack = stack<string>();    

    void postVisit(Prog &prog) override {
        assert(typeStack.size() == 0);
    }

    void postVisit(StatementExpression &exp) override {
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
        auto t1 = pop(typeStack);
        cout << "Debug: postVisit VarDecl t1: " << t1 << endl;
        // exp resault
        auto t2 = pop(typeStack);
        cout << "Debug: postVisit VarDecl t2: " << t2 << endl;
        if (t1 != t2) {
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
          
            // std::cout << "Debug: looked at: " << id.id << " Entering postVisit Id with type (VarSymbol*) " << varSymbol->type << " = " << (varSymbol->type == 0 ? "int" : "bool")
                        //   << std::endl;

        } else if (auto *funcSymbolPtr = std::get_if<FuncSymbol*>(&id.sym)) {
            FuncSymbol* funcSymbol = *funcSymbolPtr; 
            func = funcSymbol;

            // std::cout << "Debug: looked at: " << id.id << " Entering postVisit Id with type (funcSymbol*) " << funcSymbol->returnType << " = " << (funcSymbol->returnType == 0 ? "int" : "bool")
              //            << std::endl;

        } else {
            throw TypeCheckError(id.id + " is an uninitialized symbol");
        }
    }

    void preVisit(BlockLine &blockLine) override {
        hasFuncReturned = false;
    }

    void postVisit(ReturnStatement &rtn) override {
        auto t1 = pop(typeStack);
       
        string t2;
        if (func->returnType == IntType) {
            t2 = "int";
        }
        else if (func->returnType == BoolType) {
            t2 = "bool";
        }
        else {
            throw TypeCheckError("Return type of func not recognised");
        }

        if (t1 != t2) {
            throw TypeCheckError("Return type " + t2 + " does not match function return type " + t1);
        }

        hasFuncReturned = true;
    }

    void postVisit(FuncDecl &funcDecl) override {
        if (!hasFuncReturned) {
            throw TypeCheckError("Function " + funcDecl.id.id + " does not always return");
        }
        func = func->symTab->parentScope->creator;
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
        auto t1 = pop(typeStack); // lhs
        cout << "Debug: postVisit BinopExp t1: " << t1 << endl;
        // exp resault
        auto t2 = pop(typeStack); // rhs
        cout << "Debug: postVisit BinopExp t2: " << t2 << endl;

    
        if (t1 != t2) {
            throw TypeCheckError("Type of lefthand side does not match Type of righthand side");
        }
        
        auto const op = binop.op;
    
        if (t1 == "bool") {
            if (op != "&" && op != "|") {
                throw TypeCheckError(op + " does not support bools");
            }
        } // Not a bool
        else if (op == "&" || op == "|") {
            throw TypeCheckError(op + " is only supported on bools");
        }

        if (op == "==" || op == "!=" || op == "<"  || op == ">"  || op == "<=" || op == ">=") {
            typeStack.push("bool");
        } else {
            typeStack.push(t1);
        }
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


Prog typeChecker(Prog &prog) {
    auto visitor = TypeChecker();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return prog;
}








