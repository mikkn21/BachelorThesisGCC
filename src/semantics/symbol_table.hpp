#ifndef MGRAMMAR_SYMBOL_TABLE_HPP
#define MGRAMMAR_SYMBOL_TABLE_HPP 

#include "../ast.hpp"


using namespace std;


// struct ClassType {
//     string name; 
// };

struct SymbolType;

struct IntType {
    bool operator==(const IntType &other) const;
    std::string toString() const;
};

struct BoolType {
    bool operator==(const BoolType &other) const;
    std::string toString() const;
};

// TODO: Changed from std::variant to boost::variant
struct SymbolType : public boost::variant<IntType, BoolType /*, ArrayType, TypeAlias, ClassType*/> {
    // IntType,
    // BoolType
    //ArrayType
    //Class, future implementation
    using boost::variant<IntType, BoolType>::variant;
    using boost::variant<IntType, BoolType>::operator=;
    bool operator==(const SymbolType &other) const;
    bool operator!=(const SymbolType &other) const;
    string toString() const;
};



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
    unordered_map<string, Symbol*> entries;

public:

    SymbolTable *parentScope = nullptr;

    int depth;

    FuncSymbol *creator = nullptr;

    SymbolTable();

    SymbolTable(SymbolTable *parentScope);

    ~SymbolTable();

    void insert(string key, Symbol* symbol);
    Symbol *findLocal(string key) const;
    Symbol *find(string key) const;
};


#endif
