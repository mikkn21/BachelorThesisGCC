#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"
#include "../visitor.hpp"
#include "symbol_table.hpp"
#include "symbol_collection.hpp"
#include <string>
#include <variant>
#include <unordered_map>
#include <stdexcept> 

class SymbolCollectionVisitor : public Visitor {
private:
    SymbolTable *currentSymbolTable; // Has to be a pointer, not a reference!!!

public: 
    SymbolCollectionVisitor(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }

    void preVisit(Id &id) override {
        Symbol *sym = currentSymbolTable->find(id.id);
        if (sym != nullptr) {
            if (auto varSym = dynamic_cast<VarSymbol *>(sym)) {
                id.sym = varSym;
            } else if (auto funcSym = dynamic_cast<FuncSymbol *>(sym)) {
                id.sym = funcSym;
            } else {
                throw std::runtime_error("Symbol is not a VarSymbol or FuncSymbol");
            }
        }
    }

    void preVisit(VarDecl &varDecl) override {
        VarSymbol *variantSymbol = new VarSymbol(&varDecl);
        currentSymbolTable->insert(varDecl.id.id, variantSymbol);
        varDecl.sym = variantSymbol; // Mikkel addded this line I think it is correct?
    }

    void preVisit(FuncDecl &funcDecl) override {
        SymbolTable *newSymbolTable = new SymbolTable(currentSymbolTable);
        FuncSymbol *funcSymbol = new FuncSymbol(&funcDecl, newSymbolTable);

        currentSymbolTable->insert(funcDecl.id.id, funcSymbol);
        funcDecl.sym = funcSymbol;
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

