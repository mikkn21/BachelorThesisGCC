
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

    void progPostDecl(const Prog &prog) override {
        std::cout << "symbol collection prog post dec" << std::endl;
    }

    void preVarDecl(const VarDecl &varDecl) override {
        if (varDecl.type.primitive_type.type == "int") {
            Entry entry = Entry(varDecl.id.id, IntType); //should value of variable be saved?
            currentVariableTable.insert(varDecl.id.id, entry);
        }
        else if (varDecl.type.primitive_type.type == "bool") {
            Entry entry = Entry(varDecl.id.id, BoolType); //should value of variable be saved?
            currentVariableTable.insert(varDecl.id.id, entry);   
        }
        else {
            throw SemanticsError("Unknown type for variable.");
        }
    }

    void preFuncDecl(const FuncDecl &funcDecl) override {
        // Add function to currentFunctionTable
        std::vector<Parameter> tempParameters;
        for (auto i : funcDecl.parameter_list.parameter){
            tempParameters.push_back(i);
        }
        FuncEntry entry = FuncEntry(funcDecl.id.id, FuncType, tempParameters, funcDecl.type.primitive_type.type);
        currentFunctionTable.insert(funcDecl.id.id, entry);
        SymbolTable newFuncScope = SymbolTable();
        newFuncScope.parentScope = currentFunctionTable;
        // Create new scope where parent = currentFunctionScope
        // Set current scope to this one
    }

    void postFuncDecl(const FuncDecl &funcDecl) override {
        // Set current scope to parent scope
    }

};

Prog symbol_collection(Prog &prog) {
    auto visitor = SymbolCollectionVisitor();
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    return prog;
}

