#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include <iostream>
#include "symbol_collection_phase2.hpp"


class SymbolCollectionVisitorPhase2 : public Visitor {
private:
    SymbolTable *currentSymbolTable; // Has to be a pointer, not a reference!!!


public: 

    SymbolCollectionVisitorPhase2(SymbolTable *symTab) : Visitor(), currentSymbolTable(symTab) { }

    void preVisit(grammar::ast::FuncDecl &funcDecl) override {
        currentSymbolTable = funcDecl.sym->symTab;
    }

    void postVisit(grammar::ast::FuncDecl &funcDecl) override {
        currentSymbolTable = currentSymbolTable->parentScope;
    }

    void preVisit(grammar::ast::ClassDecl &classDecl) override {
        ClassSymbol *classSymbol = dynamic_cast<ClassSymbol *>(classDecl.id.sym);
        // ClassDecl shouldn't have a symbol that is not a ClassSymbol
        currentSymbolTable = classSymbol->symbolTable;
    }

    void postVisit(grammar::ast::ClassDecl &classDecl) override {
        currentSymbolTable = currentSymbolTable->parentScope;
    }

    void postVisit(grammar::ast::FunctionCall &funcCall) override { 
        Symbol *sym = currentSymbolTable->find(funcCall.id.id);
        if (sym == nullptr) {
            throw SemanticsError(funcCall.id.id + " not declared in scope5", funcCall);
        }

        if (auto funcSym = dynamic_cast<FuncSymbol *>(sym)) {
            funcCall.id.sym = funcSym;
        } else {
            throw SemanticsError("A non-function was used as a function", funcCall);
        }
    }

    void postVisit(grammar::ast::ClassType &classType) override {
        Symbol *sym = currentSymbolTable->find(classType.id.id);
        if (sym == nullptr) {
            throw SemanticsError(classType.id.id + " not declared in scope6", classType);
        }

        if (auto classSym = dynamic_cast<ClassSymbol *>(sym)) {
            classType.id.sym = classSym;
        } else {
            throw SemanticsError("A non-class was used as a class", classType);
        }
    }
    
    void postVisit(grammar::ast::VarDecl &varDecl) override {
        SymbolType symbolType = convertType(varDecl.type);
        varDecl.sym->type = symbolType;
    }

    void preVisit(grammar::ast::ObjInst &objInst) override {
        Symbol *sym = currentSymbolTable->find(objInst.id.id);
        if (sym == nullptr) {
            throw SemanticsError(objInst.id.id + " not declared in scope7", objInst);
        }
        objInst.id.sym = sym;
    }

    void postVisit(grammar::ast::IdAccess &idAccess) override {

        SymbolTable *currentScope;
        // Get the scope of the first class variable
        if (auto varSymbol = dynamic_cast<VarSymbol *>(idAccess.ids[0].sym)) {
            if (auto *classSymbolType = boost::get<ClassSymbolType>(&varSymbol->type)) {
                currentScope = classSymbolType->symbol->symbolTable;
            } else if (idAccess.ids.size() > 1) {
                    throw SemanticsError("Attempted to access a non-class", idAccess.ids[0]);
            }
        }

        // Make the sure the next IDs exist within their corresponding scopes
        // and link them up with their symbols
        for (unsigned long i = 1; i < idAccess.ids.size(); i++ ) {
            Symbol *symbol = currentScope->find(idAccess.ids[i].id);
            if (symbol == nullptr) {
                throw SemanticsError(idAccess.ids[i].id + " not declared in scope8", idAccess);
            } 
            idAccess.ids[i].sym = symbol;

            if ( auto varSymbol = dynamic_cast<VarSymbol *>(symbol)) {
                if (auto *classSymbolType = boost::get<ClassSymbolType>(&varSymbol->type)) {
                    currentScope = classSymbolType->symbol->symbolTable;
                } else if (i != idAccess.ids.size() - 1) {
                    throw SemanticsError("Attempted to access a non-class", idAccess.ids[i]);
                }
            }
        }  
    }
};

void symbol_collection_phase2(grammar::ast::Prog &prog, SymbolTable *symTab) {
    auto visitor = SymbolCollectionVisitorPhase2(symTab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

