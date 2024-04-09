#ifndef MGRAMMAR_SYMBOL_TABLE_HPP
#define MGRAMMAR_SYMBOL_TABLE_HPP 

#include "../ast.hpp"
#include <memory>


struct SymbolType;

struct IntType {
    bool operator==(const IntType &other) const;
    std::string toString() const;
};

struct BoolType {
    bool operator==(const BoolType &other) const;
    std::string toString() const;
};

struct ArraySymbolType {
    std::shared_ptr<SymbolType> elementType;
    // SymbolType *elementType; // ArrayType owns elementType
    int dimensions;
    bool operator==(const ArraySymbolType &other) const;
    std::string toString() const;
    // ~ArraySymbolType();
};

// TODO: Changed from std::variant to boost::variant
struct SymbolType : public boost::variant<IntType, BoolType, ArraySymbolType /*, TypeAlias, ClassType*/ > {
    // IntType,
    // BoolType
    //ArrayType
    //Class, future implementation
    using boost::variant<IntType, BoolType, ArraySymbolType>::variant;
    using boost::variant<IntType, BoolType, ArraySymbolType>::operator=;
    bool operator==(const SymbolType &other) const;
    bool operator!=(const SymbolType &other) const;
    std::string toString() const;
};


// struct ArrayType {
//     SymbolType elementType;
//     bool operator==(const ArrayType &other) const;
//     std::string toString() const;
// };


SymbolType convertType(grammar::ast::Type type);

class SymbolTable;

class Symbol{
public:
    virtual ~Symbol() { }
};

class FuncSymbol : public Symbol{
public:
    FuncSymbol(grammar::ast::FuncDecl *funcDecl, SymbolTable *symTab);
    ~FuncSymbol() override;
    std::vector<SymbolType> parameters;
    SymbolType returnType;
    SymbolTable *symTab;
};

class VarSymbol : public Symbol {
public:
    VarSymbol(grammar::ast::VarDecl *varDecl);
    ~VarSymbol() override { }
    SymbolType type;
    grammar::ast::VarDecl *varDecl;
    int uid;
};

// add class symbol and type symbol for aliasing, future implementation

class SymbolTable {
private:
    std::unordered_map<std::string, Symbol*> entries;

public:

    SymbolTable *parentScope = nullptr;

    int depth;

    FuncSymbol *creator = nullptr;

    SymbolTable();

    SymbolTable(SymbolTable *parentScope);

    ~SymbolTable();

    void insert(std::string key, Symbol* symbol);
    Symbol *findLocal(std::string key) const;
    Symbol *find(std::string key) const;
};


#endif
