
#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"
#include "../visitor.hpp"
#include "symbol_table.hpp"
#include "symbol_collection.hpp"
#include <string>

class SymbolCollectionVisitor : public Visitor {
private:
    SymbolTable outerVariableTable = SymbolTable();
    SymbolTable outerFunctionTable = SymbolTable();

    SymbolTable &currentVariableTable = outerVariableTable;
    SymbolTable &currentFunctionTable = outerFunctionTable;

public: 
    SymbolCollectionVisitor() : Visitor() { }

    void progPostDecl(Prog &prog) override {
        std::cout << "symbol collection prog post dec" << std::endl;
    }

    void preVarDecl(VarDecl &varDecl) override {
    }

    void preFuncDecl(FuncDecl &funcDecl) override {
        // Add function to currentFunctionTable
        //FuncSymbol symbol = FuncSymbol(&funcDecl);
        // currentFunctionTable.insert(funcDecl.id.id, symbol);
        // SymbolTable newFuncScope = SymbolTable();
        // newFuncScope.parentScope = currentFunctionTable;
        // Create new scope where parent = currentFunctionScope
        // Set current scope to this one
    }

    void postFuncDecl(FuncDecl &funcDecl) override {
        // Set current scope to parent scope
    }

};

Prog symbol_collection(Prog &prog) {
    auto visitor = SymbolCollectionVisitor();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return prog;
}

