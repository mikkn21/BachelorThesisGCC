#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include <cstddef>
#include <iostream>
#include "symbol_collection.hpp"
#include <stack>
#include <stdexcept>

template<typename T>
T pop(std::stack<T>& myStack) {
    if (myStack.empty()) {
        throw std::runtime_error("stack is empty");
    }
    T topElement = std::move(myStack.top()); 
    myStack.pop();
    return topElement;
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
    SymbolTable *currentSymbolTable; // Has to be a pointer, not a reference!!!

    // How many loops we are inside of currently in the current function.
    // There is one number for each function.
    std::stack<int> insideLoopCountStack = std::stack<int>();

    Symbol *require_id_in_scope(grammar::ast::LocationInfo &node, grammar::ast::Id &id, SymbolTable *scope) {
        Symbol *sym = currentSymbolTable->find(id.id);
        if (sym != nullptr) {
            if (auto varSym = dynamic_cast<VarSymbol *>(sym)) {
                id.sym = varSym;
            }
            return sym;
        } else {
            throw SemanticsError(id.id + " not declared in scope2", node);
        }
    }

    Symbol *require_id_in_current_scope(grammar::ast::LocationInfo &node, grammar::ast::Id &id) {
        return require_id_in_scope(node, id, currentSymbolTable);
    }

public: 

    SymbolCollectionVisitor(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }

    void pre_visit(grammar::ast::WhileStatement &whileStatement) override {
        whileStatement.start_label = generate_unique_label("while_statement");
        whileStatement.end_label = generate_unique_label("end_while_statement");
        insideLoopCountStack.top()++;
    }

    void post_visit(grammar::ast::WhileStatement &whileStatement) override {
        insideLoopCountStack.top()--;
    }

    void pre_visit(grammar::ast::BreakStatement &breakStatement) override {
        if (insideLoopCountStack.top() <= 0) {
            throw SemanticsError("Break statement outside of loop", breakStatement);
        }
    }

    void pre_visit(grammar::ast::ContinueStatement &continueStatement) override {
        if (insideLoopCountStack.top() <= 0) {
            throw SemanticsError("Continue statement outside of loop", continueStatement);
        }
    }

    void pre_visit(grammar::ast::Id &id) override {
        id.scope = currentSymbolTable;
    }

    void pre_visit(grammar::ast::VarAssign &varAssign) override {
        if (!dynamic_cast<VarSymbol *>(require_id_in_current_scope(varAssign, varAssign.id_access.ids.front()))) {
            throw SemanticsError("Attempted to assign a non-variable", varAssign);
        }
    }

    void pre_visit(grammar::ast::ClassDecl &classDecl) override {
        if (currentSymbolTable->findLocal(classDecl.id.id)) {
            throw SemanticsError(classDecl.id.id + " already declared in scope", classDecl);
        }
        SymbolTable *newSymbolTable = new SymbolTable(currentSymbolTable);
        ClassSymbol *classSymbol = new ClassSymbol(&classDecl, newSymbolTable);
        currentSymbolTable->insert(classDecl.id.id, classSymbol);
        classDecl.scope = classSymbol;
        classDecl.id.sym = classSymbol;
        currentSymbolTable = classSymbol->symbolTable;
    }


    void pre_visit(grammar::ast::VarExpression &exp) override {
        if (!dynamic_cast<VarSymbol *>(require_id_in_current_scope(exp, exp.id_access.ids.front()))) {
                throw SemanticsError("Attempted to use a non-variable as an expression", exp);
        }
    }

    void pre_visit(grammar::ast::Rhs &op_exp) override {
        op_exp.scope = currentSymbolTable;
    }

    void post_visit(grammar::ast::VarDecl &varDecl) override {
        // We use post_visit, instead of pre_visit, because then a VarExpression is visited first,
        // making sure that in the case of int x = x, then "x" in the right-hand side, is resolved in the parent scopes.
        if (currentSymbolTable->findLocal(varDecl.id.id)) {
            throw SemanticsError(varDecl.id.id + " already declared in scope", varDecl);
        }

        VarSymbol *varSymbol = new VarSymbol(&varDecl);
        currentSymbolTable->insert(varDecl.id.id, varSymbol);
        varDecl.sym = varSymbol;
        varDecl.id.sym = varSymbol;
    }

    void pre_visit(grammar::ast::FuncDecl &funcDecl) override {
        if (currentSymbolTable->findLocal(funcDecl.id.id)) {
            throw SemanticsError(funcDecl.id.id + " already declared in scope", funcDecl);
        }
        insideLoopCountStack.push(0);

        SymbolTable *newSymbolTable = new SymbolTable(currentSymbolTable);
        FuncSymbol *funcSymbol = new FuncSymbol(&funcDecl, newSymbolTable);
        funcSymbol->func_decl = &funcDecl;

        currentSymbolTable->insert(funcDecl.id.id, funcSymbol);
        funcDecl.sym = funcSymbol;
        funcDecl.id.sym = funcSymbol;
        funcDecl.label = generate_unique_label(funcDecl.id.id);
        currentSymbolTable = newSymbolTable; // Note that no object is changed, only pointers.
    }

    void post_visit(grammar::ast::FuncDecl &funcDecl) override {
        currentSymbolTable = currentSymbolTable->parentScope;
        pop(insideLoopCountStack);
    }

    void post_visit(grammar::ast::ClassDecl &decl) override {
        currentSymbolTable = currentSymbolTable->parentScope;
    }

    void post_visit(grammar::ast::ArrayIndex &index) override {
        if (!dynamic_cast<VarSymbol *>(require_id_in_current_scope(index, index.id_access.ids.front()))) {
                throw SemanticsError("Cannot index a non-variable", index);
        }
    }

    void post_visit(grammar::ast::ConditionalStatement &condStatement) override {
        condStatement.if_statement.label = generate_unique_label("if_statement");

        // Assign labels to else-if statements
        for (auto& elseIf : condStatement.else_if) {
            elseIf.label = generate_unique_label("else_if_statement");
        }

        condStatement.end_if_label = generate_unique_label("endif_statement");
        condStatement.if_statement.end_if_label = condStatement.end_if_label;

        // Set the next_label of each else-if statement to the label of the next else-if statement
        for (int i = 1; i < (int)condStatement.else_if.size(); i++) {
            condStatement.else_if[i - 1].next_label = condStatement.else_if[i].label;
            condStatement.else_if[i - 1].end_if_label = condStatement.end_if_label;
        }

        if (condStatement.conditional_else) {
            condStatement.conditional_else->label = generate_unique_label("else_statement");
            if (condStatement.else_if.size() > 0){
                condStatement.else_if.back().next_label = condStatement.conditional_else->label;
                condStatement.else_if.back().end_if_label = condStatement.end_if_label;
            }
            // Set the next_label of the if statement or the last else-if statement to the else statement label
            condStatement.if_statement.next_label = (condStatement.else_if.size() > 0) ? condStatement.else_if.front().label : condStatement.conditional_else->label;
        } else {
            if (condStatement.else_if.size() > 0){
                condStatement.else_if.back().next_label = condStatement.end_if_label;
                condStatement.else_if.back().end_if_label = condStatement.end_if_label;
            }
            // Set the next_label of the if statement or the last else-if statement to the endif label
            condStatement.if_statement.next_label = (condStatement.else_if.size() > 0) ? condStatement.else_if.front().label : condStatement.end_if_label;
        }
    }
};

void symbol_collection(grammar::ast::Prog &prog, SymbolTable *symTab) {
    auto visitor = SymbolCollectionVisitor(symTab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

