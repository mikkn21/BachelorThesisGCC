#include "../visitor.hpp"
#include "semantics_error.hpp"
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


public: 

    SymbolCollectionVisitor(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }

    void preVisit(grammar::ast::Id &id) override {
        id.scope = currentSymbolTable;
    }

    void preVisit(grammar::ast::VarAssign &varAssign) override {
        Symbol *sym = currentSymbolTable->find(varAssign.id.id);
        if (sym == nullptr) {
            throw SemanticsError(varAssign.id.id + " not declared in scope", varAssign);
        }
        varAssign.id.sym = sym;
    }

    void preVisit(grammar::ast::VarExpression &exp) override {
        Symbol *sym = currentSymbolTable->find(exp.id.id);
        if (sym != nullptr) {
            if (auto varSym = dynamic_cast<VarSymbol *>(sym)) {
                exp.id.sym = varSym;
            } else {
                throw SemanticsError("Unknown symbol type was encountered", exp);
            }
        } else {
            throw SemanticsError(exp.id.id + " not declared in scope", exp);
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

        VarSymbol *variantSymbol = new VarSymbol(&varDecl);
        
        currentSymbolTable->insert(varDecl.id.id, variantSymbol);
        varDecl.sym = variantSymbol;
        varDecl.id.sym = variantSymbol;
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

    void postVisit(grammar::ast::ArrayIndex &index) override {
        Symbol *sym = currentSymbolTable->find(index.id.id);
        if (sym == nullptr) {
            throw SemanticsError(index.id.id + " not declared in scope", index);
        }
        index.id.sym = sym;
    }

    void postVisit(grammar::ast::ConditionalStatement &condStatement) override {
        condStatement.ifStatement.label = generate_unique_label("if_statement");

        // Assign labels to else-if statements
        for (auto& elseIf : condStatement.elseIfs) {
            elseIf.label = generate_unique_label("else_if_statement");
        }

        // Set the nextLabel of each else-if statement to the label of the next else-if statement
        for (auto i = 1; i < condStatement.elseIfs.size(); i++) {
            condStatement.elseIfs[i - 1].nextLabel = condStatement.elseIfs[i].label;
        }

        condStatement.endifLabel = generate_unique_label("endif_statement");

        if (condStatement.conditionalElse) {
            condStatement.conditionalElse->label = generate_unique_label("else_statement");

            // Set the nextLabel of the if statement or the last else-if statement to the else statement label
            condStatement.ifStatement.nextLabel = (condStatement.elseIfs.size() > 0) ? condStatement.elseIfs.back().nextLabel : condStatement.conditionalElse->label;
        } else {
            // Set the nextLabel of the if statement or the last else-if statement to the endif label
            condStatement.ifStatement.nextLabel = (condStatement.elseIfs.size() > 0) ? condStatement.elseIfs.back().nextLabel : condStatement.endifLabel;
        }
    }
};

void symbol_collection(grammar::ast::Prog &prog, SymbolTable *symTab) {
    auto visitor = SymbolCollectionVisitor(symTab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

