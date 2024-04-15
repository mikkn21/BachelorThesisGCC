#include <stack>
#include "typeChecking.hpp"
#include "../visitor.hpp"
#include "../semantics/symbol_table.hpp"

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


    void postVisit(grammar::ast::Prog &prog) override {
        assert(typeStack.size() == 0);
    }

    void postVisit(grammar::ast::StatementExpression &exp) override {
        typeStack.pop();
    }

    void postVisit(grammar::ast::FunctionCall &funcCall) override {
        Symbol *sym = funcCall.id.scope->find(funcCall.id.id);
        // Symbol collection phase 2 checks that this sym is a FuncSymbol and not null
        auto funcSym = static_cast<FuncSymbol *>(sym);
        if (funcCall.argument_list.arguments.size() != funcSym->parameters.size()) {
          throw TypeCheckError("Function call does not have the correct number of arguments",funcCall);
        }

        // Parameters
        // We go backwards through the parameters because the arguments are
        // pushed onto the stack in reverse order
        for (int i = funcSym->parameters.size() - 1; i >= 0; i--) {
          SymbolType argType;
          try {
            argType = pop(typeStack);
          } catch (EmptyTypeStackError &e) {
            throw TypeCheckError("Not enough arguments for function call",
                                 funcCall);
          }

          auto paramType = funcSym->parameters[i];
          if (argType != paramType) {
            throw TypeCheckError("Argument type does not match parameter type",
                                 funcCall);
          }
        }

        typeStack.push(funcSym->returnType);
    }
    
    void postVisit(grammar::ast::VarAssign &varassign) override {
        auto varSymbol = dynamic_cast<VarSymbol *>(varassign.id.sym);

        auto t1 = varSymbol->type;
        auto t2 = pop(typeStack);

        if (t1 != t2) {
             throw TypeCheckError("Variable type do not match type of evaluated expression", varassign);
        }
    }


    // Check that the expression in the if statement evaluates to a bool
    // Checks both if and else if (since they are both if nodes in the ast)
    void postVisit(grammar::ast::IfStatement &ifStatement) override {
        // exp
        auto t1 = pop(typeStack);
        
        if (t1 != BoolType()) {
            std::ostringstream oss;
            oss << ifStatement.exp;
            throw TypeCheckError("if(" + oss.str()   + "):  do not evaluate to bool", ifStatement);
        }
    }


    void postVisit(grammar::ast::VarDeclAssign &vardecl) override {  
        //id  
        auto t1 = vardecl.decl.sym->type;
        // exp resault
        auto t2 = pop(typeStack);
        if (t1 != t2) {
            throw TypeCheckError("Type does not match expression", vardecl);
        }
    } 

    void preBlockVisit(grammar::ast::WhileStatement &whileStatement) override {
        // exp
        auto t1 = pop(typeStack);

        if (t1 != BoolType()) {
            throw TypeCheckError("Expression in while statement is not a bool", whileStatement);
        }
    }



    void postVisit(grammar::ast::IdAccess &idAccess) override {
        // check that the first |ids| - 1 are classes
        for (unsigned long i = 0; i < idAccess.ids.size()-1; i++){
            // TODO: Pointer might not be correct.

            SymbolType symType = idAccess.ids[i].sym->toType();
            if (VarSymbol *varSymbol = dynamic_cast<VarSymbol *>(idAccess.ids[i].sym)) {
                ClassSymbolType *classType = boost::get<ClassSymbolType>(&varSymbol->type);
                if (classType == nullptr) {
                    throw TypeCheckError ("Property " + idAccess.ids[i].id + " is not an object", idAccess.ids[i]);
                } 
            }
        }
        // the last id in ids can be either a class or a variable
    }


    void postVisit(grammar::ast::VarExpression &varExp) override {
        grammar::ast::Id lastId = varExp.idAccess.ids.back();
        typeStack.push(lastId.sym->toType());
    }

    void preVisit(grammar::ast::BlockLine &blockLine) override {
        hasFuncReturned = false;
    }

    void postVisit(grammar::ast::ReturnStatement &rtn) override {
        auto t1 = pop(typeStack);
        auto t2 = func->returnType;
        if (t1 != t2) {
            throw TypeCheckError("Return type " + t2.toString() + " does not match function return type " + t1.toString(), rtn);
        }

        hasFuncReturned = true;
    }

    void preVisit(grammar::ast::FuncDecl &funcDecl) override {
        func = funcDecl.sym;
    }

    void postVisit(grammar::ast::PrintStatement &_) override {
        pop(typeStack);
    }

    template <typename T>
    bool areAllInts(std::vector<T> vec) {
        auto intType = IntType();
        int size = static_cast<int>(vec.size());
        for (int i = 0; i < size; i++){
            auto type = pop(typeStack);
            if (type != intType) {
                return false;
            }
        }
        return true;
    }

    void postVisit(grammar::ast::ArrayType &arrayType) override {
        pop(typeStack);
    }

    void postVisit(grammar::ast::ArrayExp &exp) override {
        if (!areAllInts(exp.sizes)) {
            throw TypeCheckError("Array size must be an int", exp);
        }

        auto symbolType = convertType(grammar::ast::Type(exp.primType));
        typeStack.push(ArraySymbolType{std::make_shared<SymbolType>(symbolType), static_cast<int>(exp.sizes.size())});
    }

      

    void postVisit(grammar::ast::ArrayIndex &arrayIndex) override {
        VarSymbol* sym = dynamic_cast<VarSymbol *>(arrayIndex.id.sym);
        
        if (auto *type = boost::get<ArraySymbolType>(&sym->type)) {
            if (static_cast<int>(arrayIndex.indices.size()) != type->dimensions) {
                throw TypeCheckError("Indicies does not match dimensions of array", arrayIndex);
            }

            if (!areAllInts(arrayIndex.indices)) {
                throw TypeCheckError("Array index must be an int", arrayIndex);
            }

            typeStack.push(*type->elementType.get());
        } else {
            throw TypeCheckError("Index was attempted on an incompatible type", arrayIndex);
        }          
    }

    void postVisit(grammar::ast::ArrayIndexAssign &assign) override {
        // Exp result
        auto t1 = pop(typeStack);

        // Array index resault
        auto t2 = pop(typeStack);
        if (t1 != t2) {
            throw TypeCheckError("Array index type does not match expression", assign);
        }
    }

    void postVisit(grammar::ast::FuncDecl &funcDecl) override {
        if (!hasFuncReturned) {
            throw TypeCheckError("Function " + funcDecl.id.id + " does not always return", funcDecl);
        }
        func = func->symTab->parentScope->creator;
    }

    void postVisit(bool &val) override {
        typeStack.push(BoolType());
    }
    
    void postVisit(int &val) override {
        typeStack.push(IntType());
    }

    void postVisit(grammar::ast::Rhs &rhs) override {
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

    void postVisit(grammar::ast::ObjInst &inst) override {
        if (inst.arguments.arguments.size() > 0) {
            throw TypeCheckError("Object instantiation does not take arguments", inst);
        }
        
        if (auto classSymbol = dynamic_cast<ClassSymbol *>(inst.id.sym)) {
            typeStack.push(ClassSymbolType{classSymbol});
        } else {
            throw TypeCheckError("Has to be a class", inst);
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


void typeChecker(grammar::ast::Prog &prog, SymbolTable *globalScope) {
    auto visitor = TypeChecker(globalScope);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}



