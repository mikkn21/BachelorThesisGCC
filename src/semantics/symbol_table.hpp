
#ifndef MGRAMMAR_SYMBOL_TABLE_HPP
#define MGRAMMAR_SYMBOL_TABLE_HPP 

#include <unordered_map>
#include <string>
#include <optional>
#include <functional>

using namespace std;

enum EntryType {
    IntType,
    BoolType
};

class Entry {
public:
    Entry(string name, EntryType type);
    string name;
    EntryType type;
};

class IntEntry : Entry {
public:
    int value;
};

class BoolEntry : Entry {
public:
    bool value;
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
