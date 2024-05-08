#include <stack>
#include "typeChecking.hpp"
#include "../visitor.hpp"
#include "../semantics/symbol_table.hpp"
#include "return_checker.hpp"


class TypeChecker : public Visitor {
    
    // The stack of types
    std::stack<SymbolType> type_stack = std::stack<SymbolType>();    

public:
    TypeChecker() {}

private:

    // Check that there is a main function and it adheres to the rules of our main function
    void pre_visit(grammar::ast::Prog &prog) override {
        Symbol *main_symbol = prog.global_scope->find_local("main");
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
        auto func_sym = dynamic_cast<FuncSymbol *>(sym);
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
    void pre_block_visit(grammar::ast::IfStatement &if_statement) override {
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


    void post_visit(grammar::ast::ReturnStatement &rtn) override {
        auto t1 = pop(type_stack);
        auto t2 = pop(type_stack);
        // auto t2 = func->return_type;
        if (t1 != t2) {
            throw TypeCheckError("Return type " + t2.to_string() + " does not match function return type " + t1.to_string(), rtn);
        }
        type_stack.push(t2);
    }

    void pre_visit(grammar::ast::FuncDecl &func_decl) override {
        type_stack.push(func_decl.sym->return_type); 
        // func = func_decl.sym;
    }

    void post_visit(grammar::ast::PrintStatement &print) override {
        auto exp_type = pop(type_stack);
        if (exp_type != IntType() && exp_type != BoolType()) {
            throw TypeCheckError("Print statement only supports int and bool", print);
        }
    }

    template <typename T>
    bool are_all_ints(std::vector<T> vec) {
        auto int_type = IntType();
        for (size_t i = 0; i < vec.size(); i++){
            auto type = pop(type_stack);
            if (type != int_type) { // check that the dimensions are ints
                return false;
            }
        }
        return true;
    }

    void post_visit(grammar::ast::ArrayType &array_type) override {
        if (array_type.dim < 1) {
            throw TypeCheckError("Array must have at least one positive dimension", array_type);
        }
        pop(type_stack);
    }

    void post_visit(grammar::ast::ArrayInitExp &exp) override {
        if (!are_all_ints(exp.sizes)) {
            throw TypeCheckError("Array size must be an int", exp);
        }

        auto symbol_type = convert_type(grammar::ast::Type(exp.prim_type));
        type_stack.push(ArraySymbolType{std::make_shared<SymbolType>(symbol_type), static_cast<int>(exp.sizes.size())});
    }

      

    void post_visit(grammar::ast::ArrayIndex &array_index) override {
        if (array_index.id_access.ids.back().sym == nullptr) {
            throw TypeCheckError("symbol is empty in array index", array_index);
        }
        VarSymbol* sym = dynamic_cast<VarSymbol *>(array_index.id_access.ids.back().sym);
        if (sym == nullptr) {
            throw TypeCheckError("index was attempted on a non-variable", array_index);
        }
        
        if (auto *type = boost::get<ArraySymbolType>(&sym->type)) {
            if (static_cast<int>(array_index.indices.size()) != type->dimensions) {
                throw TypeCheckError("Indicies does not match dimensions of array", array_index);
            }

            if (!are_all_ints(array_index.indices)) {
                throw TypeCheckError("Array index must be an int", array_index);
            }

            type_stack.push(*type->element_type.get());
        } else {
            throw TypeCheckError("Index was attempted on an incompatible type", array_index);
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
        // if (!has_func_returned) {
        //     throw TypeCheckError("Function " + func_decl.id.id + " does not always return", func_decl);
        // }
        // func = func->sym_tab->parent_scope->creator;
        pop(type_stack);
    }

    void post_visit(bool &val) override {
        type_stack.push(BoolType());
    }
    
    void post_visit(int &val) override {
        type_stack.push(IntType());
    }

    void post_visit(grammar::ast::Rhs &rhs) override {
        auto exp_type = pop(type_stack);
        auto lhs_type = pop(type_stack);
        auto op = rhs.op;

        if (exp_type != lhs_type ) {
            throw TypeCheckError("Type of lefthand side (" + lhs_type.to_string()  + ") does not match type of righthand side (" + exp_type.to_string() + ")", rhs);
        }

        if (lhs_type == BoolType()) {
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
            type_stack.push(lhs_type);
        }
    }

    void post_visit(grammar::ast::ObjInst &inst) override {
        if (inst.arguments.arguments.size() > 0) {
            throw TypeCheckError("Object instantiation does not take arguments", inst);
        }
        
        if (auto class_symbol = dynamic_cast<ClassSymbol *>(inst.id.sym)) {
            type_stack.push(ClassSymbolType{class_symbol});
        } else {
            throw TypeCheckError("Has to be a class", inst);
        }
    }

    template<typename T>
    T pop(std::stack<T>& my_stack) {
        if (my_stack.empty()) {
            throw EmptyTypeStackError();
        }
        T top_element = std::move(my_stack.top()); 
        my_stack.pop();
        return top_element;
    } 
}; 


void type_checker(grammar::ast::Prog &prog) {
    auto visitor = TypeChecker();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);

    return_checker(prog);
}



