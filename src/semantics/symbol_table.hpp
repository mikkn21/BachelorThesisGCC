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
    int dimensions;
    bool operator==(const ArraySymbolType &other) const;
    std::string toString() const;
};

struct ClassSymbolType {
    ClassSymbol *symbol;
    bool operator==(const ClassSymbolType &other) const;
    std::string toString() const;
};

// TODO: Changed from std::variant to boost::variant
struct SymbolType : public boost::variant<IntType, BoolType, ArraySymbolType, ClassSymbolType /*, TypeAlias,*/ > {
    // IntType,
    // BoolType
    //ArrayType
    //Class, future implementation
    using boost::variant<IntType, BoolType, ArraySymbolType, ClassSymbolType>::variant;
    using boost::variant<IntType, BoolType, ArraySymbolType, ClassSymbolType>::operator=;
    bool operator==(const SymbolType &other) const;
    bool operator!=(const SymbolType &other) const;
    std::string toString() const;
};

SymbolType convertType(grammar::ast::Type type);

class SymbolTable;

class Symbol{
public:
    virtual ~Symbol() { }
    virtual SymbolType toType() = 0;
};

class FuncSymbol : public Symbol{
public:
    FuncSymbol(grammar::ast::FuncDecl *funcDecl, SymbolTable *symTab);
    ~FuncSymbol() override;
    std::vector<SymbolType> parameters;
    SymbolType returnType;
    SymbolTable *symTab;
    SymbolType toType() override;
};

class VarSymbol : public Symbol {
public:
    VarSymbol(grammar::ast::VarDecl *varDecl);
    ~VarSymbol() override { }
    SymbolType type;
    grammar::ast::VarDecl *varDecl;
    long local_id;
    SymbolType toType() override;
};

class ClassSymbol : public Symbol {
public:
    ClassSymbol(grammar::ast::ClassDecl *decl, SymbolTable *symbolTable);
    ~ClassSymbol() override;
    grammar::ast::ClassDecl *decl; // points to the class 
    SymbolTable *symbolTable;
    SymbolType toType() override;
};


// add type symbol for aliasing, future implementation

class SymbolTable {
private:
    std::unordered_map<std::string, Symbol*> entries;

public:

    SymbolTable *parentScope = nullptr;

    int depth;
    int registerCounter = 0;

    FuncSymbol *creator = nullptr;

    SymbolTable();

    SymbolTable(SymbolTable *parentScope);

    ~SymbolTable();

    void insert(std::string key, Symbol* symbol);
    void insert(std::string key, VarSymbol* symbol);

    Symbol *findLocal(std::string key) const;
    Symbol *find(std::string key) const;
    std::vector<VarSymbol*> get_var_symbols();
};


#endif
