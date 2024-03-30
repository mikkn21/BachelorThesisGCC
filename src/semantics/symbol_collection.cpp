#include <boost/variant.hpp>
#include "../ast.hpp"
#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include "symbol_collection.hpp"
#include <string>
#include <variant>
#include <boost/spirit/home/x3.hpp>
#include "../parser/parser.hpp"

class SymbolCollectionVisitor : public Visitor {
private:
    SymbolTable *currentSymbolTable; // Has to be a pointer, not a reference!!!


public: 

    SymbolCollectionVisitor(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }

    void preVisit(Id &id) override {
        id.scope = currentSymbolTable;
    }

    void preVisit(VarAssign &varAssign) override {
        Symbol *sym = currentSymbolTable->find(varAssign.id.id);
        if (sym == nullptr) {
            throw SemanticsError(varAssign.id.id + " not declared in scope", varAssign);
        }
        varAssign.id.sym = sym;
    }

    void preVisit(VarExpression &exp) override {
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

    void postVisit(VarDecl &varDecl) override {
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

    void preVisit(FuncDecl &funcDecl) override {
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

    void postVisit(FuncDecl &funcDecl) override {
        currentSymbolTable = currentSymbolTable->parentScope;
    }

};

void symbol_collection(Prog &prog, SymbolTable *symTab) {
    auto visitor = SymbolCollectionVisitor(symTab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

