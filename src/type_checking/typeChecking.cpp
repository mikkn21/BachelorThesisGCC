#include <stack>
#include <iostream>
#include <variant>
#include "typeChecking.hpp"
#include "../visitor.hpp"
#include "../semantics/symbol_table.hpp"

namespace ast = grammar::ast;


class TypeChecker : public Visitor {
    

    // the current function we are inside of
    FuncSymbol* func = nullptr;
    bool hasFuncReturned = false;

    
   
    // The stack of types
    std::stack<SymbolType> typeStack = std::stack<SymbolType>();    
    std::vector<SymbolType> FuncCallArgs = std::vector<SymbolType>();

    const SymbolTable *globalScope;

public:
    TypeChecker(SymbolTable *globalScope) : globalScope(globalScope) {}

private:

    // Check that there is a main function and it adheres to the rules of our main function
    void preVisit(grammar::ast::Prog &prog) override {
        Symbol *mainSymbol = globalScope->findLocal("main");
        if (mainSymbol == nullptr) {
            throw TypeCheckError("main function not declared");
        }

        if (auto mainFunc = dynamic_cast<FuncSymbol *>(mainSymbol)) {
            if (mainFunc->returnType != IntType()) {
                throw TypeCheckError("main function must return an int, it returns " + mainFunc->returnType.toString());
            }
            if (mainFunc->parameters.size() != 0) {
                throw TypeCheckError("main function is not allowed to have any parameters, it currently have: " + std::to_string(mainFunc->parameters.size()));
            }
        } else {
            throw TypeCheckError("main is not a function");
        }
    }


    void postVisit(ast::Prog &prog) override {
        assert(typeStack.size() == 0);
    }

    void postVisit(ast::StatementExpression &exp) override {
        typeStack.pop();
    }

    void postVisit(ast::FunctionCall &funcCall) override {
        Symbol *sym = funcCall.id.scope->find(funcCall.id.id);
        if (sym != nullptr) {
            if (dynamic_cast<VarSymbol *>(sym)) {
                throw TypeCheckError(funcCall.id.id + " variable attempted to be used as a function", funcCall);
            } else if (auto funcSym = dynamic_cast<FuncSymbol *>(sym)) {
                funcCall.id.sym = funcSym;

                if (funcCall.argument_list.arguments.size() != funcSym->parameters.size()) {
                    throw TypeCheckError("Function call does not have the correct number of arguments", funcCall);
                }

                // Parameters
                // We go backwards through the parameters because the arguments are pushed onto the stack in reverse order
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
                throw TypeCheckError("Unknown symbol type was encountered: " + std::to_string(reinterpret_cast<uintptr_t>(sym)), funcCall);
            }
        } else {
            throw TypeCheckError(funcCall.id.id + " not declared in scope", funcCall);
        }
    }
    
    void postVisit(ast::VarAssign &varassign) override {
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


    // Check that the expression in the if statement evaluates to a bool
    // Checks both if and else if (since they are both if nodes in the ast)
    // TODO: Check return statements? 
    void postVisit(ast::IfStatement &ifStatement) override {
        // exp
        auto t1 = pop(typeStack);
        
        if (t1 != BoolType()) {
            std::ostringstream oss;
            oss << ifStatement.exp;
            throw TypeCheckError("if(" + oss.str()   + "):  do not evaluate to bool", ifStatement);
        }
    }


    void postVisit(ast::VarDeclAssign &vardecl) override {  
        //id  
        auto t1 = vardecl.decl.sym->type;
        std::cout << "Debug: postVisit VarDecl t1: " << t1.toString() << std::endl;
        // exp resault
        auto t2 = pop(typeStack);
        std::cout << "Debug: postVisit VarDecl t2: " << t2.toString() << std::endl;
        if (t1 != t2) {
            throw TypeCheckError("Type does not match expression", vardecl);
        }
    } 

    void preBlockVisit(ast::WhileStatement &whileStatement) override {
        // exp
        std::cout << "Debug: Entering preBlockVisit WhileStatement" << std::endl;
        auto t1 = pop(typeStack);

        if (t1 != BoolType()) {
            throw TypeCheckError("Expression in while statement is not a bool", whileStatement);
        }
    }


    void preVisit(ast::VarExpression &varExp) override {
        std::cout << "Debug: in id: " << varExp.id.id << std::endl;
        if (varExp.id.sym == nullptr) {
            throw TypeCheckError("Symbol not found", varExp);
        }
        auto varSymbol = dynamic_cast<VarSymbol *>(varExp.id.sym);
        typeStack.push(varSymbol->type);
    }

    void preVisit(ast::BlockLine &blockLine) override {
        hasFuncReturned = false;
    }

    void postVisit(ast::ReturnStatement &rtn) override {
        auto t1 = pop(typeStack);
        // cout << "Debug: postVisit ReturnStatement t1: " << t1 << endl;
        auto t2 = func->returnType;

        // cout << "Debug: postVisit ReturnStatement t2: " << t2 << endl;

        if (t1 != t2) {
            throw TypeCheckError("Return type " + t2.toString() + " does not match function return type " + t1.toString(), rtn);
        }

        hasFuncReturned = true;
    }

    void preVisit(ast::FuncDecl &funcDecl) override {
        func = funcDecl.sym;
        // cout << "Debug: preVisit FuncDecl" << endl;
    }

    void postVisit(ast::PrintStatement &_) override {
        pop(typeStack);
    }

    template <typename T>
    bool areAllInts(std::vector<T> vec) {
        auto intType = IntType();
        int size = static_cast<int>(vec.size());
        for (int i = 0; i < size; i++){
            std::cout << "POP" << std::endl;
            auto type = pop(typeStack);
            if (type != intType) {
                return false;
            }
        }
        return true;
    }

    void postVisit(ast::ArrayExp &exp) override {
        if (!areAllInts(exp.sizes)) {
            throw TypeCheckError("Array size must be an int", exp);
        }

        auto symbolType = convertType(ast::Type(exp.primType));
        // void* memory = ::operator new(sizeof(SymbolType));
        // SymbolType* symbolTypePtr = new(memory) SymbolType(symbolType);
        typeStack.push(ArraySymbolType{std::make_shared<SymbolType>(symbolType), static_cast<int>(exp.sizes.size())});
    }


    void postVisit(ast::ArrayIndex &arrayIndex) override {
        if (auto sym = dynamic_cast<VarSymbol *>(arrayIndex.id.sym)) {
            if (auto *type = boost::get<ArraySymbolType>(&sym->type)) {
                if (static_cast<int>(arrayIndex.indices.size()) != type->dimensions) {
                    throw TypeCheckError("Index was attempted on an incompatible type", arrayIndex);
                }

                if (!areAllInts(arrayIndex.indices)) {
                    throw TypeCheckError("Array index must be an int", arrayIndex);
                }

                typeStack.push(*type->elementType.get());
            } else {
                throw TypeCheckError("Index was attempted on an incompatible type", arrayIndex);
            }
        } else {
          // TODO: Make a better msg
          throw TypeCheckError("I DUNNO MAN", arrayIndex);
          
        }
    }

    void postVisit(ast::ArrayIndexAssign &assign) override {
        // Exp result
        auto t1 = pop(typeStack);
        // std::cout << "Debug: postVisit VarDecl t1: " << t1.toString() << std::endl;

        // Array index resault
        auto t2 = pop(typeStack);
        // std::cout << "Debug: postVisit VarDecl t2: " << t2.toString() << std::endl;
        if (t1 != t2) {
            throw TypeCheckError("Array index type does not match expression", assign);
        }
    }

    void postVisit(ast::FuncDecl &funcDecl) override {
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

    void postVisit(ast::Rhs &rhs) override {
        auto expType = pop(typeStack);
        auto lhsType = pop(typeStack);
        auto op = rhs.op;

        if (expType != lhsType ) {
            throw TypeCheckError("Type of lefthand side (" + lhsType.toString()  + ") does not match type of righthand side (" + expType.toString() + ")", rhs);
        }

        if (lhsType == BoolType()) {
          if (op != "&" && op != "|" && op != "==" && op != "!=") {
            throw TypeCheckError(op + " does not support bools", rhs);
          }
        } // Not a bool
        else if (op == "&" || op == "|") {
          throw TypeCheckError(op + " is only supported on bools", rhs);
        }

        // This uses left-associativity
        if (op == "==" || op == "!=" || op == "<"  || op == ">"  || op == "<=" || op == ">=") {
            typeStack.push(BoolType());
        } else {
            typeStack.push(lhsType);
        }
    }

    template<typename T>
    T pop(std::stack<T>& myStack) {
        if (myStack.empty()) {
            throw EmptyTypeStackError();
        }
        T topElement = std::move(myStack.top()); 
        myStack.pop();
        return topElement;
    } 
}; 


ast::Prog typeChecker(ast::Prog &prog, SymbolTable *globalScope) {
    auto visitor = TypeChecker(globalScope);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return prog;
}








