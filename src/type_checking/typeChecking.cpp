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

    const SymbolTable *globalScope;

public:
    TypeChecker(SymbolTable *globalScope) : globalScope(globalScope) {}

private:

    // Check that there is a main function and it adheres to the rules of our main function
    void preVisit(Prog &prog) override {
        Symbol *mainSymbol = globalScope->findLocal("main");
        if (mainSymbol == nullptr) {
            throw TypeCheckError("main function not declared");
        }

        if (auto mainFunc = dynamic_cast<FuncSymbol *>(mainSymbol)) {
            if (mainFunc->returnType != IntType) {
                throw TypeCheckError("main function must return an int");
            }
            if (mainFunc->parameters.size() != 0) {
                throw TypeCheckError("main function is not allowed to have any parameters");
            }
        } else {
            throw TypeCheckError("main is not a function");
        }
    }

    void postVisit(Prog &prog) override {
        assert(typeStack.size() == 0);
    }

    void postVisit(StatementExpression &exp) override {
        typeStack.pop();
    }

    void preVisit(FunctionCall &funcCall) override {
        Symbol *sym = funcCall.id.scope->find(funcCall.id.id);
        if (sym != nullptr) {
            if (dynamic_cast<VarSymbol *>(sym)) {
                throw TypeCheckError(funcCall.id.id + " variable attempted to be used as a function");
            } else if (auto funcSym = dynamic_cast<FuncSymbol *>(sym)) {
                funcCall.id.sym = funcSym;
                typeStack.push(funcSym->returnType == IntType ? "int" : "bool");
            } else {
                throw TypeCheckError("Unknown symbol type was encountered");
            }
        } else {
            throw TypeCheckError(funcCall.id.id + " not declared in scope");
        }
    }
    
    void postVisit(VarAssign &varassign) override {
        // id 
        // cout << "Debug: Entering postVisit VarAssign" << endl;
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
        // cout << "Debug: Entering preBlockVisit WhileStatement" << endl;
        auto t1 = pop(typeStack);

        if (t1 != "bool") {
            throw TypeCheckError("Expression in while statement is not a bool");
        }
    }


    void postVisit(Id &id) override {
        cout << "Debug: in id: " << id.id << endl;
        if (id.sym == nullptr) {
            throw TypeCheckError("Symbol not found");
        }

        if (auto varSymbol = dynamic_cast<VarSymbol *>(id.sym)) {
            cout << "Debug: id is varsymbol" << endl;
            typeStack.push(varSymbol->type == 0 ? "int" : "bool");
        }     
    }

    void preVisit(BlockLine &blockLine) override {
        hasFuncReturned = false;
    }

    void postVisit(ReturnStatement &rtn) override {
        auto t1 = pop(typeStack);
        cout << "Debug: postVisit ReturnStatement t1: " << t1 << endl;
       
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

        cout << "Debug: postVisit ReturnStatement t2: " << t2 << endl;

        if (t1 != t2) {
            throw TypeCheckError("Return type " + t2 + " does not match function return type " + t1);
        }

        hasFuncReturned = true;
    }

    void preVisit(FuncDecl &funcDecl) override {
        func = funcDecl.sym;
        cout << "Debug: preVisit FuncDecl" << endl;
    }

    void postVisit(FuncDecl &funcDecl) override {
        if (!hasFuncReturned) {
            throw TypeCheckError("Function " + funcDecl.id.id + " does not always return");
        }
        func = func->symTab->parentScope->creator;
        cout << "Debug: postVisit FuncDecl" << endl;
    }

    void postVisit(int &value) override {
      typeStack.push("int");
      cout << "Debug: postVisit int" << endl;
    }

    void postVisit(bool &value) override {
        typeStack.push("bool");
    }


    void postVisit(BinopExp &binop) override  {
        // exp resault
        auto t1 = pop(typeStack); // lhs
        // exp resault
        auto t2 = pop(typeStack); // rhs
    
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


Prog typeChecker(Prog &prog, SymbolTable *globalScope) {
    auto visitor = TypeChecker(globalScope);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return prog;
}








