#ifndef MGRAMMAR_SYMBOL_TABLE_HPP
#define MGRAMMAR_SYMBOL_TABLE_HPP 

#include "../ast.hpp"
#include <memory>


struct SymbolType;

struct IntType {
    bool operator==(const IntType &other) const;
    std::string to_string() const;
};

struct BoolType {
    bool operator==(const BoolType &other) const;
    std::string to_string() const;
};

struct ArraySymbolType {
    std::shared_ptr<SymbolType> element_type;
    int dimensions; // TODO: should we not change this to size_t?
    bool operator==(const ArraySymbolType &other) const;
    std::string to_string() const;
};

struct ClassSymbolType {
    ClassSymbol *symbol;
    bool operator==(const ClassSymbolType &other) const;
    std::string to_string() const;
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
    std::string to_string() const;
};

SymbolType convert_type(grammar::ast::Type type);

class SymbolTable;

class Symbol{
public:
    virtual ~Symbol() { }
    virtual SymbolType to_type() = 0;
};

class FuncSymbol : public Symbol{
public:
    FuncSymbol(grammar::ast::FuncDecl *func_decl, SymbolTable *sym_tab);
    ~FuncSymbol() override;
    std::vector<SymbolType> parameters;
    SymbolType return_type;
    grammar::ast::FuncDecl *func_decl;
    SymbolTable *sym_tab;
    SymbolType to_type() override;
};

class VarSymbol : public Symbol {
public:
    VarSymbol(grammar::ast::VarDecl *var_decl);
    ~VarSymbol() override { }
    SymbolType type;
    grammar::ast::VarDecl *var_decl;
    long local_id;
    SymbolType to_type() override;
};

class ClassSymbol : public Symbol {
public:
    ClassSymbol(grammar::ast::ClassDecl *decl, SymbolTable *symbol_table);
    ~ClassSymbol() override;
    grammar::ast::ClassDecl *decl; // points to the class 
    SymbolTable *symbol_table;
    SymbolType to_type() override;
};


// add type symbol for aliasing, future implementation

class SymbolTable {
private:
    std::unordered_map<std::string, Symbol*> entries;

public:

    SymbolTable *parent_scope = nullptr;

    int depth;
    int register_counter = 0;
    int parameter_counter = 0;

    FuncSymbol *creator = nullptr;

    SymbolTable();

    SymbolTable(SymbolTable *parent_scope);

    ~SymbolTable();

    void insert(std::string key, Symbol* symbol);
    void insert(std::string key, VarSymbol* symbol);

    Symbol *find_local(std::string key) const;
    Symbol *find(std::string key) const;
    std::vector<VarSymbol*> get_var_symbols();
};


#endif
