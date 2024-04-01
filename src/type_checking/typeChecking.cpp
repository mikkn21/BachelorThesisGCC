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
    stack<SymbolType> typeStack = stack<SymbolType>();    

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
            if (mainFunc->returnType != IntType()) {
                throw TypeCheckError("main function must return an int, it returns " + mainFunc->returnType.toString());
            }
            if (mainFunc->parameters.size() != 0) {
                throw TypeCheckError("main function is not allowed to have any parameters, it currently have: " + to_string(mainFunc->parameters.size()));
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

    void postVisit(FunctionCall &funcCall) override {
        Symbol *sym = funcCall.id.scope->find(funcCall.id.id);
        if (sym != nullptr) {
            if (dynamic_cast<VarSymbol *>(sym)) {
                throw TypeCheckError(funcCall.id.id + " variable attempted to be used as a function", funcCall);
            } else if (auto funcSym = dynamic_cast<FuncSymbol *>(sym)) {
                funcCall.id.sym = funcSym;

                // Parameters
                for(int i = funcSym->parameters.size() - 1; i >= 0; i--) {
                    SymbolType argType;
                    try {
                        argType = pop(typeStack);
                    } catch (EmptyTypeStackError &e) {
                        throw TypeCheckError("Not enough arguments for function call", funcCall);
                    }

                    auto paramType = funcSym->parameters[i];
                    if (argType != paramType) {
                        throw TypeCheckError("Argument type does not match parameter type", funcCall);
                    }
                }

                typeStack.push(funcSym->returnType);
            } else {
                throw TypeCheckError("Unknown symbol type was encountered: " + to_string(reinterpret_cast<uintptr_t>(sym)), funcCall);
            }
        } else {
            throw TypeCheckError(funcCall.id.id + " not declared in scope", funcCall);
        }
    }
    
    void postVisit(VarAssign &varassign) override {
        // id 
        // cout << "Debug: Entering postVisit VarAssign" << endl;
        if (varassign.id.sym == nullptr) {
            throw TypeCheckError("Symbol not found", varassign);
        }
        auto varSymbol = dynamic_cast<VarSymbol *>(varassign.id.sym);
        auto t1 = varSymbol->type;
        // exp resault
        auto t2 = pop(typeStack);

        if (t1 != t2) {
             throw TypeCheckError("Variable type do not match type of evaluated expression", varassign);
        }
    }


    void postVisit(VarDeclAssign &vardecl) override {  
        //id  
        auto t1 = vardecl.decl.sym->type;
        // cout << "Debug: postVisit VarDecl t1: " << t1 << endl;
        // exp resault
        auto t2 = pop(typeStack);
        // cout << "Debug: postVisit VarDecl t2: " << t2 << endl;
        if (t1 != t2) {
            // cout << "Types do not mathch" << endl;
            throw TypeCheckError("Type does not match expression", vardecl);
        }
    } 

    void preBlockVisit(WhileStatement &whileStatement) override {
        // exp
        cout << "Debug: Entering preBlockVisit WhileStatement" << endl;
        auto t1 = pop(typeStack);

        if (t1 != BoolType()) {
            throw TypeCheckError("Expression in while statement is not a bool", whileStatement);
        }
    }


    void preVisit(VarExpression &varExp) override {
        cout << "Debug: in id: " << varExp.id.id << endl;
        if (varExp.id.sym == nullptr) {
            throw TypeCheckError("Symbol not found", varExp);
        }
        auto varSymbol = dynamic_cast<VarSymbol *>(varExp.id.sym);
        typeStack.push(varSymbol->type);
    }

    void preVisit(BlockLine &blockLine) override {
        hasFuncReturned = false;
    }

    void postVisit(ReturnStatement &rtn) override {
        auto t1 = pop(typeStack);
        // cout << "Debug: postVisit ReturnStatement t1: " << t1 << endl;
        auto t2 = func->returnType;

        // cout << "Debug: postVisit ReturnStatement t2: " << t2 << endl;

        if (t1 != t2) {
            throw TypeCheckError("Return type " + t2.toString() + " does not match function return type " + t1.toString(), rtn);
        }

        hasFuncReturned = true;
    }

    void preVisit(FuncDecl &funcDecl) override {
        func = funcDecl.sym;
        // cout << "Debug: preVisit FuncDecl" << endl;
    }

    void postVisit(FuncDecl &funcDecl) override {
        if (!hasFuncReturned) {
            throw TypeCheckError("Function " + funcDecl.id.id + " does not always return", funcDecl);
        }
        func = func->symTab->parentScope->creator;
        // cout << "Debug: postVisit FuncDecl" << endl;
    }

    void postVisit(bool &val) override {
        typeStack.push(BoolType());
    }
    
    void postVisit(int &val) override {
        typeStack.push(IntType());
    }

    void postVisit(BinopExp &binop) override  {
        // exp resault
        auto t1 = pop(typeStack); // lhs
        // exp resault
        auto t2 = pop(typeStack); // rhs
    
        if (t1 != t2) {
            throw TypeCheckError("Type of lefthand side does not match Type of righthand side", binop);
        }
        
        auto const op = binop.op;
    
        if (t1 == BoolType()) {
            if (op != "&" && op != "|") {
                throw TypeCheckError(op + " does not support bools", binop);
            }
        } // Not a bool
        else if (op == "&" || op == "|") {
            throw TypeCheckError(op + " is only supported on bools", binop);
        }

        if (op == "==" || op == "!=" || op == "<"  || op == ">"  || op == "<=" || op == ">=") {
            typeStack.push(BoolType());
        } else {
            typeStack.push(t1);
        }
    }

    template<typename T>
    T pop(stack<T>& myStack) {
        if (myStack.empty()) {
            throw EmptyTypeStackError();
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








