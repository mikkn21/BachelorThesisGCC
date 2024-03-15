#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"
#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include "symbol_collection.hpp"
#include <set>
#include <string>
#include <variant>
#include <unordered_map>
#include <boost/spirit/home/x3.hpp>
#include <stdexcept> 
#include "../parser/parser.hpp"

class SymbolCollectionVisitor : public Visitor {
private:
    SymbolTable *currentSymbolTable; // Has to be a pointer, not a reference!!!


public: 

    SymbolCollectionVisitor(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }


    void preVisit(Id &id) override {
        if (grammar::parser::isReserved(id.id)) {
            throw SemanticsError("Identifier is a reserved keyword");
        }


        id.scope = currentSymbolTable;

        Symbol *sym = currentSymbolTable->find(id.id);
        if (sym != nullptr) {
            if (auto varSym = dynamic_cast<VarSymbol *>(sym)) {
                id.sym = varSym;
            } else if (auto funcSym = dynamic_cast<FuncSymbol *>(sym)) {
                id.sym = funcSym;
            } else {
                throw SemanticsError(id.id + " not initialzed at this point");
            }
        } else {
            throw SemanticsError(id.id + " not declared in scope");
        }
    }

    void preVisit(VarDecl &varDecl) override {
        if (currentSymbolTable->find(varDecl.id.id)) {
            throw SemanticsError("Variable already declared in scope");
        }

        VarSymbol *variantSymbol = new VarSymbol(&varDecl);
        currentSymbolTable->insert(varDecl.id.id, variantSymbol);
        varDecl.sym = variantSymbol; // Mikkel addded this line I think it is correct?
    }

    void preVisit(FuncDecl &funcDecl) override {
        if (currentSymbolTable->find(funcDecl.id.id)) {
            throw  SemanticsError("Function already declared in scope");
        }

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

