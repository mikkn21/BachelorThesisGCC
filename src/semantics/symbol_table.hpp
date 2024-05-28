#ifndef MGRAMMAR_SYMBOL_TABLE_HPP
#define MGRAMMAR_SYMBOL_TABLE_HPP 

#include "../ast.hpp"
#include <memory>


struct SymbolType;

struct IRData {
    long stack_offset;
    bool is_local = true;
    long local_id;
};

struct IntType {
    bool operator==(const IntType &other) const;
    std::string to_string() const;
};

struct BoolType {
    bool operator==(const BoolType &other) const;
    std::string to_string() const;
};

struct BetaType {
    bool operator==(const BetaType &other) const;
    std::string to_string() const;
};

struct ArraySymbolType {
    std::shared_ptr<SymbolType> element_type;
    int dimensions;
    bool operator==(const ArraySymbolType &other) const;
    std::string to_string() const;
};

struct ClassSymbolType {
    ClassSymbol *symbol;
    bool operator==(const ClassSymbolType &other) const;
    std::string to_string() const;
};

struct FuncSymbolType {
    FuncSymbol *symbol;
    std::vector<SymbolType> parameters;
    std::shared_ptr<SymbolType> return_type;
    bool operator==(const FuncSymbolType &other) const;
    std::string to_string() const;
};

struct SymbolType : public boost::variant<IntType, BoolType, BetaType, ArraySymbolType, ClassSymbolType, FuncSymbolType /*, TypeAlias,*/ > {
    using boost::variant<IntType, BoolType, BetaType, ArraySymbolType, ClassSymbolType, FuncSymbolType>::variant;
    using boost::variant<IntType, BoolType, BetaType, ArraySymbolType, ClassSymbolType, FuncSymbolType>::operator=;
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
    grammar::ast::FuncDecl *func_decl;
    SymbolTable *sym_tab;
    FuncSymbolType type;
    SymbolType to_type() override;
};

class VarSymbol : public Symbol {
public:
    VarSymbol(grammar::ast::VarDecl *var_decl);
    ~VarSymbol() override { }
    SymbolType type;
    grammar::ast::VarDecl *var_decl;
    IRData ir_data;
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

    int depth; // TODO, should this not be size_t?
    int register_counter = 0; // TODO, should this not be size_t?
    int parameter_counter = 0; // TODO, should this not be size_t?

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
