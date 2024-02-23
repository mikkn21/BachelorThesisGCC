#include <unordered_map>
#include <string>
#include <optional>

using namespace std;

enum EntryType {
    IntType,
    BoolType
};

class Entry {
public:
    string name;
    EntryType type;

    void Accept(string *visitor) const;
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
    SymbolTable &parentScope;

    SymbolTable();

    void insert(string key, Entry &entry);
    optional<reference_wrapper<Entry>> find(string key);
};
