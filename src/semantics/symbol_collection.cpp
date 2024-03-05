#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"
#include "../visitor.hpp"
#include "symbol_table.hpp"
#include "symbol_collection.hpp"
#include <string>

class SymbolCollectionVisitor : public Visitor {
private:
    SymbolTable outerSymbolTable = new SymbolTable();

    SymbolTable &currentSymbolTable = outerSymbolTable;

public: 
    SymbolCollectionVisitor() : Visitor() { }

    void progPreDecl(Prog &prog) override {
        std::cout << "symbol collection prog post decl" << std::endl;
    }

    void progPostDecl(Prog &prog) override {
        std::cout << "symbol collection prog post decl" << std::endl;
    }

    void preVarDecl(VarDecl &varDecl) override {
        Symbol variantSymbol = &varDecl;
        std:unique_ptr<Symbol> ptr(&variantSymbol);
        currentSymbolTable.insert(varDecl.id.id, std::move(ptr));
    }

    void preFuncDecl(FuncDecl &funcDecl) override {
        // Add function to currentFunctionTable
        Symbol variantSymbol = &funcDecl;
        std:unique_ptr<Symbol> ptr(&variantSymbol);
        currentSymbolTable.insert(funcDecl.id.id, std::move(ptr));
        // Create new scope where parent = currentScope
        SymbolTable newSymbolTable = SymbolTable(&currentSymbolTable);
        // Set current scope to this one
        currentSymbolTable = &newSymbolTable; // this changes what 'currentsymbolTable' points to, not the object itself. 
        //Passing 'newSymbolTable' without '&' would change the object that 'currentSymbolTable' points to instead
    }

    void postFuncDecl(FuncDecl &funcDecl) override {
        // Set current scope to parent scope
        currentSymbolTable = currentSymbolTable.parentScope;
    }

};

Prog symbol_collection(Prog &prog) {
    auto visitor = SymbolCollectionVisitor();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return prog;
}

