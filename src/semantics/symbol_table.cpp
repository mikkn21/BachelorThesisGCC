#include "symbol_table.hpp"

#include "semantics_error.hpp"


int nextUID = 0;

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
    string operator()(const T &t) {
        return t.toString();
    }
};

struct TypeConverterVisitor : boost::static_visitor<SymbolType> {
public: 
    TypeConverterVisitor() = default; // TODO: Added this default constructor
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
};

bool SymbolType::operator==(const SymbolType &other) const {
    // return visit(SymbolTypeEqualityVisitor{}, *this, other);
    // TODO did this
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

string BoolType::toString() const {
    return "bool";
}

string IntType::toString() const {
    return "int";
}

string SymbolType::toString() const {
    // TODO: Changed this
    // return visit(SymbolTypeToStringVisitor{}, *this);
    return boost::apply_visitor(SymbolTypeToStringVisitor{}, *this);
}

SymbolType convertType(grammar::ast::Type type) {
    auto visitor = TypeConverterVisitor{}; // TODO: Default constructor on TypeConverter makes this work
    return boost::apply_visitor(visitor, type);
}

FuncSymbol::FuncSymbol(grammar::ast::FuncDecl *funcDecl, SymbolTable *scope) : symTab(scope){ 
    for (auto i : funcDecl->parameter_list.parameter){
        try {
            auto decl = boost::get<grammar::ast::VarDecl>(i);
            parameters.push_back(convertType(decl.type));
        } catch (boost::bad_get& e) {
            throw SemanticsError("Expected VarDecl in parameter list", *funcDecl);
        }
    }
    returnType = convertType(funcDecl->type);
    scope->creator = this;
}

VarSymbol::VarSymbol(grammar::ast::VarDecl *varDecl) : varDecl(varDecl) {
    uid = nextUID++;
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

