#include "symbol_table.hpp"
#include "semantics_error.hpp"
#include <iostream>
#include <stdexcept>
#include <typeinfo>

struct print_visitor {
    std::ostream& os;
    print_visitor(std::ostream& os) : os(os) {}

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


    SymbolType operator()(const grammar::ast::ArrayType &arrayType) {
        SymbolType type = (*this)(arrayType.type);
        return ArraySymbolType{std::make_shared<SymbolType>(type), arrayType.dim};
    }

    
    SymbolType operator()(const grammar::ast::ClassType &astType) {  
        auto classSymbol = dynamic_cast<ClassSymbol *>(astType.id.sym);
        // NOTE: that classSymbol might be null if the class is defined after the type is used
        return ClassSymbolType{classSymbol};
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
    return (dimensions == other.dimensions) && (*elementType.get() == *other.elementType.get());
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
    return "Array of " + elementType->to_string();
}

std::string SymbolType::to_string() const {
    return boost::apply_visitor(SymbolTypeToStringVisitor{}, *this);
}

SymbolType convertType(grammar::ast::Type type) {
    auto visitor = AstTypeConverterVisitor{};
    return boost::apply_visitor(visitor, type);
}

FuncSymbol::FuncSymbol(grammar::ast::FuncDecl *funcDecl, SymbolTable *scope) : sym_tab(scope){ 
    for (auto i : funcDecl->parameter_list.parameter){
        try {
            auto decl = boost::get<grammar::ast::VarDecl>(i);
            parameters.push_back(convertType(decl.type));
        } catch (boost::bad_get& e) {
            throw SemanticsError("Expected VarDecl in parameter list", *funcDecl);
        }
    }
    return_type = convertType(funcDecl->type);
    scope->creator = this;
}


ClassSymbol::ClassSymbol(grammar::ast::ClassDecl *decl, SymbolTable *scope) : decl(decl), symbolTable(scope){ 
}
 
VarSymbol::VarSymbol(grammar::ast::VarDecl *varDecl) : varDecl(varDecl) {
    type = convertType(varDecl->type);
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
    delete(symbolTable);
}

// Outer-most scope
SymbolTable::SymbolTable() : depth(0) { }

// One of the inner scopes
SymbolTable::SymbolTable(SymbolTable *parentScope) : parentScope(parentScope), depth(parentScope->depth + 1) { }

SymbolTable::~SymbolTable(){
    for (const auto &i : entries){
        delete(i.second);
    }
}

void SymbolTable::insert(std::string key, VarSymbol* symbol) {
    if (symbol->varDecl->id.scope->creator != nullptr) {
        auto& parameter_list = symbol->varDecl->id.scope->creator->parameters;
        auto it = std::find(parameter_list.begin(), parameter_list.end(), symbol->type);

        if (it != parameter_list.end()) {
            symbol->local_id = --parameterCounter; // is a parameter
        } else {
            symbol->local_id = ++registerCounter; // is not a parameter, but a local variable
        }
    } else {
            symbol->local_id = ++registerCounter; // is not a parameter, but a local variable
    }

    entries.emplace(key, symbol); 
}

void SymbolTable::insert(std::string key, Symbol *symbol) {
    entries.emplace(key, symbol); 
}

Symbol *SymbolTable::findLocal(std::string key) const {
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
        if (parentScope == nullptr) {
            return nullptr;
        } else {
            return parentScope->find(key);
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
