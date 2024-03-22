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

struct TypeConverterVisitor : boost::static_visitor<SymbolType> {
    SymbolType operator()(const PrimitiveType &t) {
        auto const type = t.type;
        if (type == "int") {
            return IntType;
        }
        else if (type == "bool"){
            return BoolType;
        }

        throw SemanticsError("Unknown primitive type");
    }
};

SymbolType convertType(Type type) {
    auto visitor = TypeConverterVisitor{};
    return boost::apply_visitor(visitor, type);
}

FuncSymbol::FuncSymbol(FuncDecl *funcDecl, SymbolTable *scope) : symTab(scope){ 
    for (auto i : funcDecl->parameter_list.parameter){
        parameters.push_back(convertType(i.type));
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

void SymbolTable::insert(string key, Symbol *symbol) {
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
