#include "symbol_table.hpp"
#include "semantics_error.hpp"
#include <iostream>
#include <stdexcept>
#include <typeinfo>

struct PrintVisitor {
    std::ostream& os;
    PrintVisitor(std::ostream& os) : os(os) {}

    template<typename T>
    void operator()(const T& t) const {
        os << t;
    }
};

struct SymbolTypeEqualityVisitor {
    template <typename T, typename E>
    bool operator()(const T &t, const E &e) const { // TODO: added const
        return false;
    }

    template <typename T>
    bool operator()(const T &t1, const T &t2) const { // TODO: added const
        return t1 == t2;
    }

};

struct SymbolTypeToStringVisitor {
    template <typename T>
    std::string operator()(const T &t) {
        return t.to_string();
    }
};

struct AstTypeConverterVisitor : boost::static_visitor<SymbolType> {
public: 
    AstTypeConverterVisitor() = default; // TODO: Added this default constructor

    SymbolType operator()(const grammar::ast::PrimitiveType &t) {
        auto const type = t.type;
        if (type == "int") {
            return IntType();
        }
        else if (type == "bool"){
            return BoolType();
        }

        throw SemanticsError("Unknown primitive type", t);
    }


    SymbolType operator()(const grammar::ast::ArrayType &array_type) {
        SymbolType type = (*this)(array_type.type);
        return ArraySymbolType{std::make_shared<SymbolType>(type), array_type.dim};
    }

    
    SymbolType operator()(const grammar::ast::ClassType &ast_type) {  
        auto class_symbol = dynamic_cast<ClassSymbol *>(ast_type.id.sym);
        // NOTE: that class_symbol might be null if the class is defined after the type is used
        return ClassSymbolType{class_symbol};
    }
};

bool SymbolType::operator==(const SymbolType &other) const {
    return boost::apply_visitor(SymbolTypeEqualityVisitor{}, *this, other);
}


bool SymbolType::operator!=(const SymbolType &other) const {
    return !(*this == other);
}

bool BoolType::operator==(const BoolType &other) const {
    return true;
}

bool IntType::operator==(const IntType &other) const {
    return true;
}

bool ClassSymbolType::operator==(const ClassSymbolType &other) const {
    return symbol->decl->id.id == other.symbol->decl->id.id;
}

bool ArraySymbolType::operator==(const ArraySymbolType &other) const {
    return (dimensions == other.dimensions) && (*element_type.get() == *other.element_type.get());
}

std::string BoolType::to_string() const {
    return "bool";
}

std::string IntType::to_string() const {
    return "int";
}

std::string ClassSymbolType::to_string() const {
    return "class<" + symbol->decl->id.id + ">";
}

std::string ArraySymbolType::to_string() const {
    return "Array of " + element_type->to_string();
}

std::string SymbolType::to_string() const {
    return boost::apply_visitor(SymbolTypeToStringVisitor{}, *this);
}

SymbolType convert_type(grammar::ast::Type type) {
    auto visitor = AstTypeConverterVisitor{};
    return boost::apply_visitor(visitor, type);
}

FuncSymbol::FuncSymbol(grammar::ast::FuncDecl *func_decl, SymbolTable *scope) : sym_tab(scope){ 
    for (auto i : func_decl->parameter_list.parameter){
        try {
            auto decl = boost::get<grammar::ast::VarDecl>(i);
            parameters.push_back(convert_type(decl.type));
        } catch (boost::bad_get& e) {
            throw SemanticsError("Expected VarDecl in parameter list", *func_decl);
        }
    }
    return_type = convert_type(func_decl->type);
    scope->creator = this;
}


ClassSymbol::ClassSymbol(grammar::ast::ClassDecl *decl, SymbolTable *scope) : decl(decl), symbol_table(scope){ 
}
 
VarSymbol::VarSymbol(grammar::ast::VarDecl *var_decl) : var_decl(var_decl) {
    type = convert_type(var_decl->type);
}

SymbolType FuncSymbol::to_type() {
    throw std::runtime_error("FuncSymbol cannot be converted to a SymbolType");
}

SymbolType ClassSymbol::to_type() {
    return ClassSymbolType{this};
}

SymbolType VarSymbol::to_type() {
    return type;
}

FuncSymbol::~FuncSymbol() {
    delete(sym_tab);
}

ClassSymbol::~ClassSymbol() {
    delete(symbol_table);
}

// Outer-most scope
SymbolTable::SymbolTable() : depth(0) { }

// One of the inner scopes
SymbolTable::SymbolTable(SymbolTable *parent_scope) : parent_scope(parent_scope), depth(parent_scope->depth + 1) { }

SymbolTable::~SymbolTable(){
    for (const auto &i : entries){
        delete(i.second);
    }
}

void SymbolTable::insert(std::string key, VarSymbol* symbol) {
    if (symbol->var_decl->id.scope->creator != nullptr) {
        auto& parameter_list = symbol->var_decl->id.scope->creator->parameters;
        auto it = std::find(parameter_list.begin(), parameter_list.end(), symbol->type);

        if (it != parameter_list.end()) {
            symbol->local_id = --parameter_counter; // is a parameter
        } else {
            symbol->local_id = ++register_counter; // is not a parameter, but a local variable
        }
    } else {
            symbol->local_id = ++register_counter; // is not a parameter, but a local variable
    }

    entries.emplace(key, symbol); 
}

void SymbolTable::insert(std::string key, Symbol *symbol) {
    entries.emplace(key, symbol); 
}

Symbol *SymbolTable::find_local(std::string key) const {
    auto x = entries.find(key);
    if (x == entries.end()) {
        return nullptr;
    }
    else {
        return x->second;
    }
}

Symbol *SymbolTable::find(std::string key) const {
    auto x = entries.find(key);
    if (x == entries.end()) {
        if (parent_scope == nullptr) {
            return nullptr;
        } else {
            return parent_scope->find(key);
        }
    }
    else {
        return x->second;
    }
}

std::vector<VarSymbol*> SymbolTable::get_var_symbols() {
    std::vector<VarSymbol*> var_symbols;
    for (auto var : entries) {
        if (VarSymbol* var_casted = dynamic_cast<VarSymbol*>(var.second)) {
            var_symbols.push_back(var_casted);
        } 
    }
    return var_symbols;
}
