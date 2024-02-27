
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
    BoolType,
    ArrayType
    //Class, future implementation
};

class FuncSymbol{
public:
    FuncSymbol(FuncDecl *funcDecl);
    std::vector<SymbolType> parameters;
    SymbolType returnType;
    FuncDecl *funcDecl;
};

class VarSymbol {
public:
    VarSymbol(VarDecl *varDecl);
    SymbolType type;
    VarDecl *varDecl;    
};

using Symbol = std::variant<FuncSymbol, VarSymbol>; // add class symbol and type symbol for aliasing, future implementation

class SymbolTable {
private:
    unordered_map<string, std::unique_ptr<Symbol>> entries;

public:
    SymbolTable *parentScope = nullptr;

    SymbolTable();

    SymbolTable(SymbolTable *parentScope);

    void insert(string key, std::unique_ptr<Symbol> symbol);
    Symbol *find(string key);
};

#endif
