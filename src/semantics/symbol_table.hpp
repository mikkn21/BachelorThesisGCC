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


// struct ClassType {
//     string name; 
// };

struct SymbolType;

struct IntType {
    bool operator==(const IntType &other) const;
    string toString() const;
};

struct BoolType {
    bool operator==(const BoolType &other) const;
    string toString() const;
};

struct SymbolType : public std::variant<IntType, BoolType /*, ArrayType, TypeAlias, ClassType*/> {
    // IntType,
    // BoolType
    //ArrayType
    //Class, future implementation
    using std::variant<IntType, BoolType>::variant;
    using std::variant<IntType, BoolType>::operator=;
    bool operator==(const SymbolType &other) const;
    bool operator!=(const SymbolType &other) const;
    string toString() const;
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
    long local_id;
};

// add class symbol and type symbol for aliasing, future implementation

class SymbolTable {
private:
    unordered_map<string, Symbol*> entries;

public:

    SymbolTable *parentScope = nullptr;

    int depth;
    int registerCounter = 0;

    FuncSymbol *creator = nullptr;

    SymbolTable();

    SymbolTable(SymbolTable *parentScope);

    ~SymbolTable();

    void insert(string key, FuncSymbol* symbol);
    void insert(string key, VarSymbol* symbol);

    Symbol *findLocal(string key) const;
    Symbol *find(string key) const;
    vector<VarSymbol*> get_var_symbols();
};


#endif
