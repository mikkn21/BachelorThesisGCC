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
        
        throw SemanticsError("Unknown type");
    }
};

SymbolType convertType(Type type) {
    auto visitor = TypeConverterVisitor{};
    return boost::apply_visitor(visitor, type);
}

FuncSymbol::FuncSymbol(FuncDecl *funcDecl, std::unique_ptr<SymbolTable> scope) : funcDecl(funcDecl), symTab(std::move(scope)){ 
    for (auto i : funcDecl->parameter_list.parameter){
        parameters.push_back(convertType(i.type));
    }
    returnType = convertType(funcDecl->type);
}

VarSymbol::VarSymbol(VarDecl *varDecl) : varDecl(varDecl) {
    type = convertType(varDecl->type);
}

SymbolTable::SymbolTable() = default;

SymbolTable::SymbolTable(SymbolTable *parentScope) : parentScope(nullptr) { }

SymbolTable::~SymbolTable() = default;

void SymbolTable::insert(string key, std::unique_ptr<Symbol> symbol) {
    entries.insert({key, std::move(symbol)}); 
}

Symbol *SymbolTable::find(string key) {
    auto x = entries.find(key);
    if (x == entries.end()) {
        return nullptr;
    }
    else {
        return x->second.get();
    }
}

std::ostream& operator<<(std::ostream& os, FuncSymbol symbol){
    return os << " func, returntype: " << symbol.returnType;
}

std::ostream& operator<<(std::ostream& os, VarSymbol symbol){
    return os << " var, type: " << symbol.type;
}

void SymbolTable::print() {
    for (auto i = entries.begin() ; i != entries.end() ; i++){
        std::cout << i->first << "\t"; 
        std::visit(print_visitor(std::cout), *(i->second));
        std::cout << std::endl;
    }
}