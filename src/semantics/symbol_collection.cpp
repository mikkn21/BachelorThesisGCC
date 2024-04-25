#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include <cstddef>
#include <iostream>
#include "symbol_collection.hpp"
#include <stack>
#include <stdexcept>

template<typename T>
T pop(std::stack<T>& my_stack) {
    if (my_stack.empty()) {
        throw std::runtime_error("stack is empty");
    }
    T top_element = std::move(my_stack.top()); 
    my_stack.pop();
    return top_element;
} 

size_t unique_label_id = 0;

std::string generate_unique_label(std::string name) {
    if (name != "main") {
        return "L"+ std::to_string(unique_label_id++) + "_" + name;
    }
    return name;
    
}

class SymbolCollectionVisitor : public Visitor {
private:
    SymbolTable *current_symbol_table; // Has to be a pointer, not a reference!!!

    // How many loops we are inside of currently in the current function.
    // There is one number for each function.
    std::stack<int> inside_loop_count_stack = std::stack<int>();

    Symbol *require_id_in_scope(grammar::ast::LocationInfo &node, grammar::ast::Id &id, SymbolTable *scope) {
        Symbol *sym = current_symbol_table->find(id.id);
        if (sym != nullptr) {
            if (auto var_sym = dynamic_cast<VarSymbol *>(sym)) {
                id.sym = var_sym;
            }
            return sym;
        } else {
            throw SemanticsError(id.id + " not declared in scope2", node);
        }
    }

    Symbol *require_id_in_current_scope(grammar::ast::LocationInfo &node, grammar::ast::Id &id) {
        return require_id_in_scope(node, id, current_symbol_table);
    }

public: 

    SymbolCollectionVisitor(SymbolTable *sym_tab) : Visitor(), current_symbol_table(sym_tab) { }

    void pre_visit(grammar::ast::WhileStatement &while_statement) override {
        while_statement.start_label = generate_unique_label("while_statement");
        while_statement.end_label = generate_unique_label("end_while_statement");
        inside_loop_count_stack.top()++;
    }

    void post_visit(grammar::ast::WhileStatement &while_statement) override {
        inside_loop_count_stack.top()--;
    }

    void pre_visit(grammar::ast::BreakStatement &break_statement) override {
        if (inside_loop_count_stack.top() <= 0) {
            throw SemanticsError("Break statement outside of loop", break_statement);
        }
    }

    void pre_visit(grammar::ast::ContinueStatement &continue_statement) override {
        if (inside_loop_count_stack.top() <= 0) {
            throw SemanticsError("Continue statement outside of loop", continue_statement);
        }
    }

    void pre_visit(grammar::ast::Id &id) override {
        id.scope = current_symbol_table;
    }

    void pre_visit(grammar::ast::ArrayInitExp &init) override {
        init.scope = current_symbol_table;
    }

    void pre_visit(grammar::ast::VarAssign &var_assign) override {
        if (!dynamic_cast<VarSymbol *>(require_id_in_current_scope(var_assign, var_assign.id_access.ids.front()))) {
            throw SemanticsError("Attempted to assign a non-variable", var_assign);
        }
    }

    void pre_visit(grammar::ast::ClassDecl &class_decl) override {
        if (current_symbol_table->find_local(class_decl.id.id)) {
            throw SemanticsError(class_decl.id.id + " already declared in scope", class_decl);
        }
        SymbolTable *new_symbol_table = new SymbolTable(current_symbol_table);
        ClassSymbol *class_symbol = new ClassSymbol(&class_decl, new_symbol_table);
        current_symbol_table->insert(class_decl.id.id, class_symbol);
        class_decl.scope = class_symbol;
        class_decl.id.sym = class_symbol;
        current_symbol_table = class_symbol->symbol_table;
    }


    void pre_visit(grammar::ast::VarExpression &exp) override {
        if (!dynamic_cast<VarSymbol *>(require_id_in_current_scope(exp, exp.id_access.ids.front()))) {
            throw SemanticsError("Attempted to use a non-variable as an expression", exp);
        }
    }

    void pre_visit(grammar::ast::Rhs &op_exp) override {
        op_exp.scope = current_symbol_table;
    }

    void post_visit(grammar::ast::VarDecl &var_decl) override {
        // We use post_visit, instead of pre_visit, because then a VarExpression is visited first,
        // making sure that in the case of int x = x, then "x" in the right-hand side, is resolved in the parent scopes.
        if (current_symbol_table->find_local(var_decl.id.id)) {
            throw SemanticsError(var_decl.id.id + " already declared in scope", var_decl);
        }

        VarSymbol *var_symbol = new VarSymbol(&var_decl);
        current_symbol_table->insert(var_decl.id.id, var_symbol);
        var_decl.sym = var_symbol;
        var_decl.id.sym = var_symbol;
    }

    void pre_visit(grammar::ast::FuncDecl &func_decl) override {
        if (current_symbol_table->find_local(func_decl.id.id)) {
            throw SemanticsError(func_decl.id.id + " already declared in scope", func_decl);
        }
        inside_loop_count_stack.push(0);

        SymbolTable *new_symbol_table = new SymbolTable(current_symbol_table);
        FuncSymbol *func_symbol = new FuncSymbol(&func_decl, new_symbol_table);
        func_symbol->func_decl = &func_decl;

        current_symbol_table->insert(func_decl.id.id, func_symbol);
        func_decl.sym = func_symbol;
        func_decl.id.sym = func_symbol;
        func_decl.label = generate_unique_label(func_decl.id.id);
        current_symbol_table = new_symbol_table; // Note that no object is changed, only pointers.
    }

    void post_visit(grammar::ast::FuncDecl &func_decl) override {
        current_symbol_table = current_symbol_table->parent_scope;
        pop(inside_loop_count_stack);
    }

    void post_visit(grammar::ast::ClassDecl &decl) override {
        current_symbol_table = current_symbol_table->parent_scope;
    }

    void post_visit(grammar::ast::ArrayIndex &index) override {
        if (!dynamic_cast<VarSymbol *>(require_id_in_current_scope(index, index.id_access.ids.front()))) {
                throw SemanticsError("Cannot index a non-variable", index);
        }
    }

    void post_visit(grammar::ast::ConditionalStatement &cond_statement) override {
        cond_statement.if_statement.label = generate_unique_label("if_statement");

        // Assign labels to else-if statements
        for (auto& elseIf : cond_statement.else_if) {
            elseIf.label = generate_unique_label("else_if_statement");
        }

        cond_statement.end_if_label = generate_unique_label("endif_statement");
        cond_statement.if_statement.end_if_label = cond_statement.end_if_label;

        // Set the next_label of each else-if statement to the label of the next else-if statement
        for (int i = 1; i < (int)cond_statement.else_if.size(); i++) {
            cond_statement.else_if[i - 1].next_label = cond_statement.else_if[i].label;
            cond_statement.else_if[i - 1].end_if_label = cond_statement.end_if_label;
        }

        if (cond_statement.conditional_else) {
            cond_statement.conditional_else->label = generate_unique_label("else_statement");
            if (cond_statement.else_if.size() > 0){
                cond_statement.else_if.back().next_label = cond_statement.conditional_else->label;
                cond_statement.else_if.back().end_if_label = cond_statement.end_if_label;
            }
            // Set the next_label of the if statement or the last else-if statement to the else statement label
            cond_statement.if_statement.next_label = (cond_statement.else_if.size() > 0) ? cond_statement.else_if.front().label : cond_statement.conditional_else->label;
        } else {
            if (cond_statement.else_if.size() > 0){
                cond_statement.else_if.back().next_label = cond_statement.end_if_label;
                cond_statement.else_if.back().end_if_label = cond_statement.end_if_label;
            }
            // Set the next_label of the if statement or the last else-if statement to the endif label
            cond_statement.if_statement.next_label = (cond_statement.else_if.size() > 0) ? cond_statement.else_if.front().label : cond_statement.end_if_label;
        }
    }
};

void symbol_collection(grammar::ast::Prog &prog, SymbolTable *sym_tab) {
    auto visitor = SymbolCollectionVisitor(sym_tab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

