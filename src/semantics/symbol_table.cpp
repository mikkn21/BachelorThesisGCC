#include <string>
#include <optional>
#include <unordered_map>
#include "symbol_table.hpp"
#include <functional>
#include "../ast.hpp"
#include <variant>
#include "semantics_error.hpp"
#include <memory>
#include <iostream>
#include <typeinfo>

using namespace std;

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
    bool operator()(const T &t, const E &e) {
        return false;
    }

    template <typename T>
    bool operator()(const T &t1, const T &t2) {
        return t1 == t2;
    }
};

struct SymbolTypeToStringVisitor {
    template <typename T>
    string operator()(const T &t) {
        return t.toString();
    }
};

struct TypeConverterVisitor : boost::static_visitor<SymbolType> {
    SymbolType operator()(PrimitiveType &t) { // I removed the const keyword here, is it a problem? - sofus
        auto const type = t.type;
        if (type == "int") {
            return IntType();
        }
        else if (type == "bool"){
            return BoolType();
        }

        throw SemanticsError("Unknown primitive type", t);
    }
};

bool SymbolType::operator==(const SymbolType &other) const {
    return visit(SymbolTypeEqualityVisitor{}, *this, other);
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

string BoolType::toString() const {
    return "bool";
}

string IntType::toString() const {
    return "int";
}

string SymbolType::toString() const {
    return visit(SymbolTypeToStringVisitor{}, *this);
}

SymbolType convertType(Type type) {
    auto visitor = TypeConverterVisitor{};
    return boost::apply_visitor(visitor, type);
}

FuncSymbol::FuncSymbol(FuncDecl *funcDecl, SymbolTable *scope) : symTab(scope){ 
    for (auto i : funcDecl->parameter_list.parameter){
        try {
            auto decl = boost::get<VarDecl>(i);
            parameters.push_back(convertType(decl.type));
        } catch (boost::bad_get& e) {
            throw SemanticsError("Expected VarDecl in parameter list", *funcDecl);
        }
    }
    returnType = convertType(funcDecl->type);
    scope->creator = this;
}

VarSymbol::VarSymbol(VarDecl *varDecl) : varDecl(varDecl) {
    type = convertType(varDecl->type);
}


FuncSymbol::~FuncSymbol() {
    delete(symTab);
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

void SymbolTable::insert(string key, VarSymbol* symbol) {
    symbol->local_id = ++registerCounter;
    entries.emplace(key, symbol); 
}

void SymbolTable::insert(string key, FuncSymbol *symbol) {
    entries.emplace(key, symbol); 
}

Symbol *SymbolTable::findLocal(string key) const {
    auto x = entries.find(key);
    if (x == entries.end()) {
        return nullptr;
    }
    else {
        return x->second;
    }
}

Symbol *SymbolTable::find(string key) const {
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

vector<VarSymbol*> SymbolTable::get_var_symbols() {
    vector<VarSymbol*> var_symbols;
    for (auto var : entries) {
        auto var_casted = dynamic_cast<VarSymbol*>(var.second);
        if (typeid(var_casted) == typeid(VarSymbol*)) {
            var_symbols.push_back(var_casted);
        } else {
            cout << "something went wrong when casting symbol" << endl;
        }
    }
    return var_symbols;
}
