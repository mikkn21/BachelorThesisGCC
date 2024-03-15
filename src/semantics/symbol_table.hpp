#ifndef MGRAMMAR_SYMBOL_TABLE_HPP
#define MGRAMMAR_SYMBOL_TABLE_HPP 

#include <unordered_map>
#include <string>
#include <optional>
#include <functional>
#include <variant>
#include "../ast.hpp"
#include <memory>
#include <iostream>

using namespace std;

enum SymbolType {
    IntType,
    BoolType
    //ArrayType
    //Class, future implementation
};

SymbolType convertType(Type type);

class SymbolTable;

class Symbol{
public:
    virtual ~Symbol() { }
};

class FuncSymbol : public Symbol{
public:
    FuncSymbol(FuncDecl *funcDecl, SymbolTable *symTab);
    ~FuncSymbol() override;
    std::vector<SymbolType> parameters;
    SymbolType returnType;
    SymbolTable *symTab;
};

class VarSymbol : public Symbol {
public:
    VarSymbol(VarDecl *varDecl);
    ~VarSymbol() override { }
    SymbolType type;
    VarDecl *varDecl;
    int uid;
};

// add class symbol and type symbol for aliasing, future implementation

class SymbolTable {
private:
    unordered_map<string, Symbol*> entries;

public:

    SymbolTable *parentScope = nullptr;

    int depth;

    FuncSymbol *creator = nullptr;

    SymbolTable();

    SymbolTable(SymbolTable *parentScope);

    ~SymbolTable();

    void insert(string key, Symbol* symbol);
    Symbol *findLocal(string key);
    Symbol *find(string key);
};

#endif
