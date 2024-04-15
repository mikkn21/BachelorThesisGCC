#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include "symbol_collection.hpp"


class SymbolCollectionVisitor : public Visitor {
private:
    SymbolTable *currentSymbolTable; // Has to be a pointer, not a reference!!!


public: 

    SymbolCollectionVisitor(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }

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

        currentSymbolTable->insert(funcDecl.id.id, funcSymbol);
        funcDecl.sym = funcSymbol;
        funcDecl.id.sym = funcSymbol;
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
};

void symbol_collection(grammar::ast::Prog &prog, SymbolTable *symTab) {
    auto visitor = SymbolCollectionVisitor(symTab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

