#include <string>
#include <optional>
#include <unordered_map>
#include "symbol_table.hpp"
#include <functional>
#include "../ast.hpp"
#include <variant>
#include <boost/variant.hpp>
#include "semantics_error.hpp"
#include <memory>

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

FuncSymbol::FuncSymbol(FuncDecl *funcDecl) : funcDecl(funcDecl){ 
    for (auto i : funcDecl->parameter_list.parameter){
        parameters.push_back(convertType(i.type));
    }
    returnType = convertType(funcDecl->type);
}

VarSymbol::VarSymbol(VarDecl *varDecl) : varDecl(varDecl) {
    type = convertType(varDecl->type);
}

SymbolTable::SymbolTable() { }

SymbolTable::SymbolTable(SymbolTable *parentScope) : parentScope(parentScope) { }

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
