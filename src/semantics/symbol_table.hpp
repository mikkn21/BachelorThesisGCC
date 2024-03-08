#ifndef MGRAMMAR_SYMBOL_TABLE_HPP
#define MGRAMMAR_SYMBOL_TABLE_HPP 

#include <unordered_map>
#include <string>
#include <optional>
#include <functional>
#include <variant>
#include "../ast.hpp"
#include <memory>

using namespace std;

enum SymbolType {
    IntType,
    BoolType
    //ArrayType
    //Class, future implementation
};

class SymbolTable;

class Symbol{

};

class FuncSymbol : public Symbol{
public:
    FuncSymbol(FuncDecl *funcDecl, SymbolTable *symTab);
    ~FuncSymbol();
    std::vector<SymbolType> parameters;
    SymbolType returnType;
    FuncDecl *funcDecl;
    SymbolTable *symTab;
};

class VarSymbol : public Symbol {
public:
    VarSymbol(VarDecl *varDecl);
    SymbolType type;
    VarDecl *varDecl;    
};

// add class symbol and type symbol for aliasing, future implementation

class SymbolTable {
private:
    

public:
    unordered_map<string, Symbol*> entries;

    SymbolTable *parentScope = nullptr;

    SymbolTable();

    SymbolTable(SymbolTable *parentScope);

    ~SymbolTable();

    void insert(string key, Symbol *symbol);
    Symbol *find(string key);

    void print();
};

#endif
