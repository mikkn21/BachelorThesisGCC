#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include <cstddef>
#include <iostream>
#include "symbol_collection.hpp"

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

    // How many loops we are inside of currently
    int insideLoopCount = 0;


public: 

    SymbolCollectionVisitor(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }

    void preVisit(grammar::ast::WhileStatement &whileStatement) override {
        whileStatement.start_label = generate_unique_label("while_statement");
        whileStatement.end_label = generate_unique_label("end_while_statement");
        insideLoopCount++;
    }

    void postVisit(grammar::ast::WhileStatement &whileStatement) override {
        insideLoopCount--;
    }

    void preVisit(grammar::ast::BreakStatement &breakStatement) override {
        if (insideLoopCount <= 0) {
            throw SemanticsError("Break statement outside of loop", breakStatement);
        }
    }

    void preVisit(grammar::ast::ContinueStatement &continueStatement) override {
        if (insideLoopCount <= 0) {
            throw SemanticsError("Continue statement outside of loop", continueStatement);
        }
    }

    void preVisit(grammar::ast::Id &id) override {
        id.scope = currentSymbolTable;
    }

    void preVisit(grammar::ast::VarAssign &varAssign) override {
        Symbol *sym = currentSymbolTable->find(varAssign.idAccess.ids.front().id);
        if (sym == nullptr) {
            throw SemanticsError(varAssign.idAccess.ids.front().id + " not declared in scope4", varAssign);
        }
        varAssign.idAccess.ids.front().sym = sym;
    }

    void preVisit(grammar::ast::ClassDecl &classDecl) override {
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


    void preVisit(grammar::ast::VarExpression &exp) override {
        grammar::ast::Id varId = exp.idAccess.ids[0];
        Symbol *sym = currentSymbolTable->find(varId.id);
        if (sym != nullptr) {
            if (auto varSym = dynamic_cast<VarSymbol *>(sym)) {
                varId.sym = varSym;
            } else {
                throw SemanticsError("Unknown symbol type was encountered", exp);
            }
        } else {
            throw SemanticsError(varId.id + " not declared in scope2", exp);
        }
    }

    void preVisit(grammar::ast::Rhs &op_exp) override {
        op_exp.scope = currentSymbolTable;
    }

    void postVisit(grammar::ast::VarDecl &varDecl) override {
        // We use postVisit, instead of preVisit, because then a VarExpression is visited first,
        // making sure that in the case of int x = x, then "x" in the right-hand side, is resolved in the parent scopes.
        if (currentSymbolTable->findLocal(varDecl.id.id)) {
            throw SemanticsError(varDecl.id.id + " already declared in scope", varDecl);
        }

        VarSymbol *varSymbol = new VarSymbol(&varDecl);
        currentSymbolTable->insert(varDecl.id.id, varSymbol);
        varDecl.sym = varSymbol;
        varDecl.id.sym = varSymbol;
    }

    void preVisit(grammar::ast::FuncDecl &funcDecl) override {
        if (currentSymbolTable->findLocal(funcDecl.id.id)) {
            throw SemanticsError(funcDecl.id.id + " already declared in scope", funcDecl);
        }

        SymbolTable *newSymbolTable = new SymbolTable(currentSymbolTable);
        FuncSymbol *funcSymbol = new FuncSymbol(&funcDecl, newSymbolTable);
        funcSymbol->funcDecl = &funcDecl;

        currentSymbolTable->insert(funcDecl.id.id, funcSymbol);
        funcDecl.sym = funcSymbol;
        funcDecl.id.sym = funcSymbol;
        funcDecl.label = generate_unique_label(funcDecl.id.id);
        currentSymbolTable = newSymbolTable; // Note that no object is changed, only pointers.
    }

    void postVisit(grammar::ast::FuncDecl &funcDecl) override {
        currentSymbolTable = currentSymbolTable->parentScope;
    }

    void postVisit(grammar::ast::ClassDecl &decl) override {
        currentSymbolTable = currentSymbolTable->parentScope;
    }

    void postVisit(grammar::ast::ArrayIndex &index) override {
        Symbol *symbol = currentSymbolTable->find(index.idAccess.ids.front().id);
        if (symbol == nullptr) {
            throw SemanticsError(index.idAccess.ids.front().id + " not declared in scope3", index);
        }
    }

    void postVisit(grammar::ast::ConditionalStatement &condStatement) override {
        condStatement.ifStatement.label = generate_unique_label("if_statement");

        // Assign labels to else-if statements
        for (auto& elseIf : condStatement.elseIfs) {
            elseIf.label = generate_unique_label("else_if_statement");
        }

        condStatement.endIfLabel = generate_unique_label("endif_statement");
        condStatement.ifStatement.endIfLabel = condStatement.endIfLabel;

        // Set the nextLabel of each else-if statement to the label of the next else-if statement
        for (int i = 1; i < static_cast<int>(condStatement.elseIfs.size()); i++) {
            condStatement.elseIfs[i - 1].nextLabel = condStatement.elseIfs[i].label;
            condStatement.elseIfs[i - 1].endIfLabel = condStatement.endIfLabel;
        }        

        if (condStatement.conditionalElse) {
            condStatement.conditionalElse->label = generate_unique_label("else_statement");

            // Set the nextLabel of the if statement or the last else-if statement to the else statement label
            condStatement.ifStatement.nextLabel = (condStatement.elseIfs.size() > 0) ? condStatement.elseIfs.back().nextLabel : condStatement.conditionalElse->label;
        } else {
            // Set the nextLabel of the if statement or the last else-if statement to the endif label
            condStatement.ifStatement.nextLabel = (condStatement.elseIfs.size() > 0) ? condStatement.elseIfs.back().nextLabel : condStatement.endIfLabel;
        }
    }
};

void symbol_collection(grammar::ast::Prog &prog, SymbolTable *symTab) {
    auto visitor = SymbolCollectionVisitor(symTab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

