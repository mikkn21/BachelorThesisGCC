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

    SymbolTable *currentSymbolTable; // Has to be a pointer, not a reference!!!

public: 
    SymbolCollectionVisitor(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }

    void preVisit(VarDecl &varDecl) override {
        VarSymbol *variantSymbol = new VarSymbol(&varDecl);
        //currentSymbolTable.insert(varDecl.id.id, variantSymbol);
        currentSymbolTable->entries.emplace(varDecl.id.id, variantSymbol);
    }

    void preVisit(FuncDecl &funcDecl) override {
        std::cout << "pre function" << std::endl;
        // Create new scope where parent = currentScope
        SymbolTable *newSymbolTable = new SymbolTable(currentSymbolTable);
        //Passing 'newSymbolTable' without '&' would change the object that 'currentSymbolTable' points to instead
        std::cout << "1 function" << std::endl;
        
        if (newSymbolTable->parentScope == nullptr) {
            std::cout << "NULL" << std::endl;
        }
        else {
            std::cout << "NOT NULL" << std::endl;
        }
        
        FuncSymbol *funcSym = new FuncSymbol(&funcDecl, newSymbolTable);

        if (funcSym->symTab == nullptr) {
            std::cout << "NULL 2" << std::endl;
        }
        else {
            std::cout << "NOT NULL 2" << std::endl;
        }
        std::cout << "2 function" << std::endl;
        // Add function to currentFunctionTable
        currentSymbolTable->entries.emplace(funcDecl.id.id, funcSym);
        std::cout << currentSymbolTable->entries.size() << std::endl;
        std::cout << "3 function" << std::endl;
        // Set current scope to this one
        funcDecl.sym = funcSym;
        std::cout << "4 function" << std::endl;
        currentSymbolTable = newSymbolTable; // this changes what 'currentsymbolTable' points to, not the object itself. 
        std::cout << "5 function" << std::endl;
    }

    void postVisit(FuncDecl &funcDecl) override {
        std::cout << "post function" << std::endl;
        // Set current scope to parent scope
        std::cout << currentSymbolTable->entries.size() << std::endl;

        if (currentSymbolTable->parentScope == nullptr) {
            std::cout << "aoeu" << std::endl;
        }

        currentSymbolTable = currentSymbolTable->parentScope;
        std::cout << "post function 2" << std::endl;
    }

};

void symbol_collection(Prog &prog, SymbolTable *symTab) {
    auto visitor = SymbolCollectionVisitor(symTab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

