#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"
#include "../visitor.hpp"
#include "symbol_table.hpp"
#include "symbol_collection.hpp"
#include <string>
#include <variant>
#include <unordered_map>

class SymbolCollectionVisitor : public Visitor {
private:
    //SymbolTable const * outerSymbolTable = new SymbolTable();

    SymbolTable &currentSymbolTable;

public: 
    SymbolCollectionVisitor(SymbolTable &symTab) : Visitor(), currentSymbolTable(symTab) { }

    void preVisit(Prog &prog) override {
        std::cout << "symbol collection prog post decl" << std::endl;
    }

    void postVisit(Prog &prog) override {
        std::cout << "symbol collection prog post decl" << std::endl;
    }

    void preVisit(VarDecl &varDecl) override {
        VarSymbol *variantSymbol = new VarSymbol(&varDecl);
        //currentSymbolTable.insert(varDecl.id.id, variantSymbol);
        currentSymbolTable.entries.emplace(varDecl.id.id, variantSymbol);
    }

    void preVisit(FuncDecl &funcDecl) override {
        // Create new scope where parent = currentScope
        SymbolTable newSymbolTable = new SymbolTable(&currentSymbolTable);
        // Set current scope to this one
        currentSymbolTable = &newSymbolTable; // this changes what 'currentsymbolTable' points to, not the object itself. 
        //Passing 'newSymbolTable' without '&' would change the object that 'currentSymbolTable' points to instead
        
        FuncSymbol *funcSym = new FuncSymbol(&funcDecl, &newSymbolTable);
        // Add function to currentFunctionTable
        currentSymbolTable.entries.emplace(funcDecl.id.id, funcSym);
    }

    void postVisit(FuncDecl &funcDecl) override {
        // Set current scope to parent scope
        currentSymbolTable = currentSymbolTable.parentScope;
    }

};

Prog symbol_collection(Prog &prog, SymbolTable &symTab) {
    auto visitor = SymbolCollectionVisitor(symTab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return prog;
}

