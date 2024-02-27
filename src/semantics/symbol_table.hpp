
#ifndef MGRAMMAR_SYMBOL_TABLE_HPP
#define MGRAMMAR_SYMBOL_TABLE_HPP 

#include <unordered_map>
#include <string>
#include <optional>
#include <functional>
#include "../ast.hpp"

using namespace std;

enum EntryType {
    IntType,
    BoolType,
    FuncType
};

class Entry {
public:
    Entry(string name, EntryType type);
    string name;
    EntryType type;
};

class IntEntry : public Entry {
public:
    int value;
};

class BoolEntry : public Entry {
public:
    bool value;
};

class FuncEntry : public Entry{
public:
    FuncEntry(string name, EntryType type, std::vector<Parameter> parameters, string returnType);
    std::vector<Parameter> parameters;
    string returnType;
};

class SymbolTable {
private:
    unordered_map<string, Entry> entries;

public:
    optional<reference_wrapper<SymbolTable>> parentScope;

    SymbolTable();

    SymbolTable(SymbolTable &parent);

    void insert(string key, Entry &entry);
    optional<reference_wrapper<Entry>> find(string key);
};

#endif
