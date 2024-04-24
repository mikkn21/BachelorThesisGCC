#include <stack>
#include "typeChecking.hpp"
#include "../visitor.hpp"
#include "../semantics/symbol_table.hpp"

class TypeChecker : public Visitor {

    // the current function we are inside of
    FuncSymbol* func = nullptr;
    bool has_func_returned = false;

    // The stack of types
    std::stack<SymbolType> type_stack = std::stack<SymbolType>();    
    std::vector<SymbolType> func_call_args = std::vector<SymbolType>();

    const SymbolTable *global_scope;

public:
    TypeChecker(SymbolTable *global_scope) : global_scope(global_scope) {}

private:

    // Check that there is a main function and it adheres to the rules of our main function
    void pre_visit(grammar::ast::Prog &prog) override {
        Symbol *main_symbol = global_scope->findLocal("main");
        if (main_symbol == nullptr) {
            throw TypeCheckError("main function not declared");
        }

        if (auto main_func = dynamic_cast<FuncSymbol *>(main_symbol)) {
            if (main_func->return_type != IntType()) {
                throw TypeCheckError("main function must return an int, it returns " + main_func->return_type.to_string());
            }
            if (main_func->parameters.size() != 0) {
                throw TypeCheckError("main function is not allowed to have any parameters, it currently have: " + std::to_string(main_func->parameters.size()));
            }
        } else {
            throw TypeCheckError("main is not a function");
        }
    }


    void post_visit(grammar::ast::Prog &prog) override {
        assert(type_stack.size() == 0);
    }

    void post_visit(grammar::ast::StatementExpression &exp) override {
        type_stack.pop();
    }

    void post_visit(grammar::ast::FunctionCall &func_call) override {
        Symbol *sym = func_call.id.scope->find(func_call.id.id);
        // Symbol collection phase 2 checks that this sym is a FuncSymbol and not null
        auto func_sym = static_cast<FuncSymbol *>(sym);
        if (func_call.argument_list.arguments.size() != func_sym->parameters.size()) {
          throw TypeCheckError("Function call does not have the correct number of arguments",func_call);
        }

        // Parameters
        // We go backwards through the parameters because the arguments are
        // pushed onto the stack in reverse order
        for (size_t i = func_sym->parameters.size(); i > 0; i--) {
          SymbolType arg_type;
          try {
            arg_type = pop(type_stack);
          } catch (EmptyTypeStackError &e) {
            throw TypeCheckError("Not enough arguments for function call",
                                 func_call);
          }

          auto param_type = func_sym->parameters[i-1];
          if (arg_type != param_type) {
            throw TypeCheckError("Argument type does not match parameter type",
                                 func_call);
          }
        }

        type_stack.push(func_sym->return_type);
    }
    
    void post_visit(grammar::ast::VarAssign &var_assign) override {
        auto var_symbol = dynamic_cast<VarSymbol *>(var_assign.id_access.ids.back().sym);

        auto t1 = var_symbol->type;
        auto t2 = pop(type_stack);

        if (t1 != t2) {
             throw TypeCheckError("Variable type do not match type of evaluated expression", var_assign);
        }
    }


    // Check that the expression in the if statement evaluates to a bool
    // Checks both if and else if (since they are both if nodes in the ast)
    void post_visit(grammar::ast::IfStatement &if_statement) override {
        // exp
        auto t1 = pop(type_stack);
        
        if (t1 != BoolType()) {
            std::ostringstream oss;
            oss << if_statement.exp;
            throw TypeCheckError("if(" + oss.str()   + "):  do not evaluate to bool", if_statement);
        }
    }


    void post_visit(grammar::ast::VarDeclAssign &var_decl) override {  
        //id  
        auto t1 = var_decl.decl.sym->type;
        // exp resault
        auto t2 = pop(type_stack);
        if (t1 != t2) {
            throw TypeCheckError("Type does not match expression", var_decl);
        }
    } 

    void pre_block_visit(grammar::ast::WhileStatement &while_statement) override {
        // exp
        auto t1 = pop(type_stack);

        if (t1 != BoolType()) {
            throw TypeCheckError("Expression in while statement is not a bool", while_statement);
        }
    }


    //<---- here
    void post_visit(grammar::ast::IdAccess &id_access) override {
        // check that the first |ids| - 1 are classes
        for (size_t i = 0; i < id_access.ids.size()-1; i++){
            // TODO: Pointer might not be correct.

            SymbolType sym_type = id_access.ids[i].sym->to_type();
            if (VarSymbol *var_symbol = dynamic_cast<VarSymbol *>(id_access.ids[i].sym)) {
                ClassSymbolType *class_type = boost::get<ClassSymbolType>(&var_symbol->type);
                if (class_type == nullptr) {
                    throw TypeCheckError ("Property " + id_access.ids[i].id + " is not an object", id_access.ids[i]);
                } 
            }
        }
        // the last id in ids can be either a class or a variable
    }


    void post_visit(grammar::ast::VarExpression &var_exp) override {
        grammar::ast::Id last_id = var_exp.id_access.ids.back();
        type_stack.push(last_id.sym->to_type());
    }

    void pre_visit(grammar::ast::BlockLine &block_line) override {
        has_func_returned = false;
    }

    void post_visit(grammar::ast::ReturnStatement &rtn) override {
        auto t1 = pop(type_stack);
        auto t2 = func->return_type;
        if (t1 != t2) {
            throw TypeCheckError("Return type " + t2.to_string() + " does not match function return type " + t1.to_string(), rtn);
        }

        has_func_returned = true;
    }

    void pre_visit(grammar::ast::FuncDecl &func_decl) override {
        func = func_decl.sym;
    }

    void post_visit(grammar::ast::PrintStatement &_) override {
        pop(type_stack);
    }

    template <typename T>
    bool areAllInts(std::vector<T> vec) {
        auto int_type = IntType();
        int size = static_cast<int>(vec.size());
        for (int i = 0; i < size; i++){
            auto type = pop(type_stack);
            if (type != int_type) {
                return false;
            }
        }
        return true;
    }

    void post_visit(grammar::ast::ArrayType &arrayType) override {
        pop(type_stack);
    }

    void post_visit(grammar::ast::ArrayExp &exp) override {
        if (!areAllInts(exp.sizes)) {
            throw TypeCheckError("Array size must be an int", exp);
        }

        auto symbolType = convertType(grammar::ast::Type(exp.prim_type));
        type_stack.push(ArraySymbolType{std::make_shared<SymbolType>(symbolType), static_cast<int>(exp.sizes.size())});
    }

      

    void post_visit(grammar::ast::ArrayIndex &arrayIndex) override {
        if (arrayIndex.id_access.ids.back().sym == nullptr) {
            throw TypeCheckError("symbol is empty in array index", arrayIndex);
        }
        VarSymbol* sym = dynamic_cast<VarSymbol *>(arrayIndex.id_access.ids.back().sym);
        if (sym == nullptr) {
            throw TypeCheckError("index was attempted on a non-variable", arrayIndex);
        }
        
        if (auto *type = boost::get<ArraySymbolType>(&sym->type)) {
            if (static_cast<int>(arrayIndex.indices.size()) != type->dimensions) {
                throw TypeCheckError("Indicies does not match dimensions of array", arrayIndex);
            }

            if (!areAllInts(arrayIndex.indices)) {
                throw TypeCheckError("Array index must be an int", arrayIndex);
            }

            type_stack.push(*type->elementType.get());
        } else {
            throw TypeCheckError("Index was attempted on an incompatible type", arrayIndex);
        }          
    }

    void post_visit(grammar::ast::ArrayIndexAssign &assign) override {
        // Exp result
        auto t1 = pop(type_stack);

        // Array index resault
        auto t2 = pop(type_stack);
        if (t1 != t2) {
            throw TypeCheckError("Array index type does not match expression", assign);
        }
    }

    void post_visit(grammar::ast::FuncDecl &func_decl) override {
        if (!has_func_returned) {
            throw TypeCheckError("Function " + func_decl.id.id + " does not always return", func_decl);
        }
        func = func->sym_tab->parentScope->creator;
    }

    void post_visit(bool &val) override {
        type_stack.push(BoolType());
    }
    
    void post_visit(int &val) override {
        type_stack.push(IntType());
    }

    void post_visit(grammar::ast::Rhs &rhs) override {
        auto expType = pop(type_stack);
        auto lhsType = pop(type_stack);
        auto op = rhs.op;

        if (expType != lhsType ) {
            throw TypeCheckError("Type of lefthand side (" + lhsType.to_string()  + ") does not match type of righthand side (" + expType.to_string() + ")", rhs);
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
            type_stack.push(BoolType());
        } else {
            type_stack.push(lhsType);
        }
    }

    void post_visit(grammar::ast::ObjInst &inst) override {
        if (inst.arguments.arguments.size() > 0) {
            throw TypeCheckError("Object instantiation does not take arguments", inst);
        }
        
        if (auto classSymbol = dynamic_cast<ClassSymbol *>(inst.id.sym)) {
            type_stack.push(ClassSymbolType{classSymbol});
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


void typeChecker(grammar::ast::Prog &prog, SymbolTable *global_scope) {
    auto visitor = TypeChecker(global_scope);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}



