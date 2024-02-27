#include <string>
#include <optional>
#include <unordered_map>
#include "symbol_table.hpp"
#include <functional>

Entry::Entry(string name, EntryType type) : name(name), type(type) { }

FuncEntry::FuncEntry(string name, EntryType type, std::vector<Parameter> parameters, string returnType) : Entry(name, type), parameters(parameters), returnType(returnType) { }

SymbolTable::SymbolTable() { }

SymbolTable::SymbolTable(SymbolTable &parentScope) : parentScope(parentScope) { }

void SymbolTable::insert(string key, Entry &entry) {
    entries.insert({key, entry});
}

optional<reference_wrapper<Entry>> SymbolTable::find(string key) {
    auto x = entries.find(key);
    if (x == entries.end()) {
        return std::nullopt;
    }
    else {
        return x->second;
    }
}
