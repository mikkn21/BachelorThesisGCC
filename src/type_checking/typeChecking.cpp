#include <stack>
#include "typeChecking.hpp"
#include "../visitor.hpp"
#include "../semantics/symbol_table.hpp"
#include "return_checker.hpp"
#include <iostream>
#include <memory>


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
            if (*main_func->type.return_type.get() != IntType()) {
                throw TypeCheckError("main function must return an int, it returns " + main_func->type.return_type.get()->to_string());
            }
            if (main_func->type.parameters.size() != 0) {
                throw TypeCheckError("main function is not allowed to have any parameters, it currently have: " + std::to_string(main_func->type.parameters.size()));
            }
        } else {
            throw TypeCheckError("main is not a function");
        }
    }


    void post_visit(grammar::ast::Prog &prog) override {
        // print_stack(type_stack);
        assert(type_stack.size() == 0);
    }

    void post_visit(grammar::ast::Parameter &param) override {
        auto t1 = pop(type_stack);
    }
    
    void post_visit(grammar::ast::VarDeclStatement &var_decl) override {
        pop(type_stack);
    }

    void post_visit(grammar::ast::StatementExpression &exp) override {
        pop(type_stack);
    }

    void pre_attr_visit(grammar::ast::ClassDecl &class_decl) override {
        pop(type_stack);
    }

    void post_visit(grammar::ast::ClassType &type) override {
        if (type.id.sym == nullptr) {
            throw TypeCheckError("Class type is empty", type);
        }
        pop(type_stack);
    }

    void post_visit(grammar::ast::BetaExpression &beta) override {
        type_stack.push(BetaType());
    }

    void post_visit(grammar::ast::FunctionCall &func_call) override {

        // Get arguments types from stack
        std::vector<SymbolType> args;
        for (size_t i = 0; i < func_call.argument_list.arguments.size(); i++) {
            args.push_back(pop(type_stack));
        }

        // NOTE: The arguments are in reverse order because of the stack
        
        // Get function type from stack
        auto type = pop(type_stack);

        FuncSymbolType func_type;
        if (auto *f_type = boost::get<FuncSymbolType>(&type)) {
            func_type = *f_type;
        } else {
            throw TypeCheckError("Function call on a non-function", func_call);
        }

        if (args.size() != func_type.parameters.size()) {
          throw TypeCheckError("Function call does not have the correct number of arguments",func_call);
        }


        // Parameters
        // We go backwards through the parameters because the arguments are
        // pushed onto the stack in reverse order
        for (size_t i = func_type.parameters.size(); i > 0; i--) {
            SymbolType arg_type = args[args.size() - i]; // The arguments are in reverse order because of the stack

            auto param_type = func_type.parameters[i-1]; // the parameters are in the correct order
            if (arg_type != param_type) {
                throw TypeCheckError("Argument type does not match parameter type",
                                    func_call);
            }
        }
        type_stack.push(*func_type.return_type.get());
    }
    
    void post_visit(grammar::ast::VarAssign &var_assign) override {
        auto t1 = pop(type_stack);
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
        // id  
        auto t1 = pop(type_stack);
        // exp
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

    void post_visit(grammar::ast::Id &id) override {
        if (id.sym == nullptr) {
            throw TypeCheckError("Symbol is empty", id);
        }
        type_stack.push(id.sym->to_type());
    }


    void post_visit(grammar::ast::IdAccess &id_access) override {
        // the last id in ids can be either a class or a variable
        auto last_id_type = pop(type_stack);
        // check that the first |ids| - 1 are classes
        for (size_t i = 0; i < id_access.ids.size()-1; i++){
            auto t1 = pop(type_stack);
            if (boost::get<ClassSymbolType>(&t1) == nullptr) {
                throw TypeCheckError("Property " + id_access.ids[i].id + " is not an object", id_access.ids[i]);
            }
        }
        type_stack.push(last_id_type);
    }


    void post_visit(grammar::ast::ReturnStatement &rtn) override {
        auto t1 = pop(type_stack); // Exp
        auto t2 = pop(type_stack); // Return type of function
        if (t1 != t2) {
            throw TypeCheckError("Return type " + t2.to_string() + " does not match function return type " + t1.to_string(), rtn);
        }
        type_stack.push(t2);
    }

    void pre_visit(grammar::ast::FuncDecl &func_decl) override {
        auto type = func_decl.sym->type.return_type.get();
        type_stack.push(*type); 
    }

    void pre_parameter_list_visit(grammar::ast::FuncDecl &func_decl) override {
        pop(type_stack); // Id
    }

    void post_visit(grammar::ast::FuncDecl &func_decl) override {
        pop(type_stack); // Return type
    }

    void post_visit(grammar::ast::PrintStatement &print) override {
        auto exp_type = pop(type_stack);
        if (exp_type != IntType() && exp_type != BoolType() && exp_type != BetaType()) {
            throw TypeCheckError("Print statements only supports ints, bools, objects and arrays", print);
        }
        print.input_type = std::make_shared<SymbolType>(exp_type);
    }

    bool are_all_ints(std::vector<SymbolType> types) {
        auto int_type = IntType();
        for (auto type : types){
            if (type != int_type) {
                return false;
            }
        }
        return true;
    }

    void post_visit(grammar::ast::ArrayType &array_type) override {
        if (array_type.dim < 1) {
            throw TypeCheckError("Array must have at least one positive dimension", array_type);
        }
        pop(type_stack); // Dimension (which is an int)
    }

    void post_visit(grammar::ast::ArrayInitExp &exp) override {
        std::vector<SymbolType> type_sizes(exp.sizes.size());
        for(size_t i = 0; i < exp.sizes.size(); i++) {
            type_sizes.push_back(pop(type_stack));
        }

        if (!are_all_ints(type_sizes)) {
            throw TypeCheckError("Array size must be an int", exp);
        }

        auto symbol_type = convert_type(grammar::ast::Type(exp.prim_type));
        type_stack.push(ArraySymbolType{std::make_shared<SymbolType>(symbol_type), static_cast<int>(exp.sizes.size())});
    }

    void post_visit(grammar::ast::ArrayIndex &array_index) override {

        // Get the types of the indices form the stack (they are before the array index type in the stack)
        std::vector<SymbolType> indices_type(array_index.indices.size());
        for(size_t i = 0; i < array_index.indices.size(); i++) {
            indices_type.push_back(pop(type_stack));
        }

        // get array type
        auto array_type = pop(type_stack);
        
        if (auto *type = boost::get<ArraySymbolType>(&array_type)) {
            if (static_cast<int>(array_index.indices.size()) != type->dimensions) {
                throw TypeCheckError("Indicies does not match dimensions of array", array_index);
            }

            if (!are_all_ints(indices_type)) {
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

        // Array index result
        auto t2 = pop(type_stack);
        if (t1 != t2) {
            throw TypeCheckError("Array index type does not match expression", assign);
        }
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

        auto t1 = pop(type_stack);
        try { 
            boost::get<ClassSymbolType>(t1);
        } catch (boost::bad_get& e) {
            throw TypeCheckError("Has to be a class", inst);
        }
        type_stack.push(t1);
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

    template<typename T>
    void print_stack(std::stack<T> &type_stack) {
        if (type_stack.size() != 0) {
            std::string stack_content = "Stack content: \n";
            stack_content += "======================\n";
            size_t i = 0;
            while (!type_stack.empty()) {
                auto e = pop(type_stack);
                stack_content += "Element " + std::to_string(i) +": " + e.to_string() + "\n";
                ++i;
            }
            stack_content += "======================\n";
            std::cout << stack_content << std::endl;
            throw TypeCheckError("Type stack is not empty");
        }
    }
}; 




void type_checker(grammar::ast::Prog &prog) {
    auto visitor = TypeChecker();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);

    return_checker(prog);
}



