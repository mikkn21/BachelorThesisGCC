#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include <iostream>
#include "symbol_collection_phase2.hpp"


class SymbolCollectionVisitorPhase2 : public Visitor {
private:
    SymbolTable *current_symbol_table; // Has to be a pointer, not a reference!!!


public: 

    SymbolCollectionVisitorPhase2(SymbolTable *sym_tab) : Visitor(), current_symbol_table(sym_tab) { }

    void pre_visit(grammar::ast::FuncDecl &funcDecl) override {
        current_symbol_table = funcDecl.sym->sym_tab;
    }

    void post_visit(grammar::ast::FuncDecl &funcDecl) override {
        current_symbol_table = current_symbol_table->parent_scope;
    }

    void pre_visit(grammar::ast::ClassDecl &classDecl) override {
        ClassSymbol *classSymbol = dynamic_cast<ClassSymbol *>(classDecl.id.sym);
        // ClassDecl shouldn't have a symbol that is not a ClassSymbol
        current_symbol_table = classSymbol->symbol_table;
    }

    void post_visit(grammar::ast::ClassDecl &classDecl) override {
        current_symbol_table = current_symbol_table->parent_scope;
    }

    void post_visit(grammar::ast::FunctionCall &funcCall) override { 
        Symbol *sym = current_symbol_table->find(funcCall.id.id);
        if (sym == nullptr) {
            throw SemanticsError(funcCall.id.id + " not declared in scope5", funcCall);
        }

        if (auto funcSym = dynamic_cast<FuncSymbol *>(sym)) {
            funcCall.id.sym = funcSym;
        } else {
            throw SemanticsError("A non-function was used as a function", funcCall);
        }
    }

    void post_visit(grammar::ast::ClassType &classType) override {
        Symbol *sym = current_symbol_table->find(classType.id.id);
        if (sym == nullptr) {
            throw SemanticsError(classType.id.id + " not declared in scope6", classType);
        }

        if (auto classSym = dynamic_cast<ClassSymbol *>(sym)) {
            classType.id.sym = classSym;
        } else {
            throw SemanticsError("A non-class was used as a class", classType);
        }
    }
    
    void post_visit(grammar::ast::VarDecl &varDecl) override {
        SymbolType symbolType = convert_type(varDecl.type);
        varDecl.sym->type = symbolType;
    }

    void pre_visit(grammar::ast::ObjInst &objInst) override {
        Symbol *sym = current_symbol_table->find(objInst.id.id);
        if (sym == nullptr) {
            throw SemanticsError(objInst.id.id + " not declared in scope7", objInst);
        }
        objInst.id.sym = sym;
    }

    void post_visit(grammar::ast::IdAccess &id_access) override {

        SymbolTable *currentScope;
        // Get the scope of the first class variable
        if (auto varSymbol = dynamic_cast<VarSymbol *>(id_access.ids[0].sym)) {
            if (auto *classSymbolType = boost::get<ClassSymbolType>(&varSymbol->type)) {
                currentScope = classSymbolType->symbol->symbol_table;
            } else if (id_access.ids.size() > 1) {
                    throw SemanticsError("Attempted to access a non-class", id_access.ids[0]);
            }
        }

        // Make the sure the next IDs exist within their corresponding scopes
        // and link them up with their symbols
        for (unsigned long i = 1; i < id_access.ids.size(); i++ ) {
            Symbol *symbol = currentScope->find(id_access.ids[i].id);
            if (symbol == nullptr) {
                throw SemanticsError(id_access.ids[i].id + " not declared in scope8", id_access);
            } 
            id_access.ids[i].sym = symbol;

            if ( auto varSymbol = dynamic_cast<VarSymbol *>(symbol)) {
                if (auto *classSymbolType = boost::get<ClassSymbolType>(&varSymbol->type)) {
                    currentScope = classSymbolType->symbol->symbol_table;
                } else if (i != id_access.ids.size() - 1) {
                    throw SemanticsError("Attempted to access a non-class", id_access.ids[i]);
                }
            }
        }  
    }
};

void symbol_collection_phase2(grammar::ast::Prog &prog, SymbolTable *sym_tab) {
    auto visitor = SymbolCollectionVisitorPhase2(sym_tab);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

