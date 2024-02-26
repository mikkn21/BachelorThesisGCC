
#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"

using boost::apply_visitor;

class Visitor {
public: 
    Visitor() {}

    virtual void ProgPreDecl(const Prog &prog) { }
    virtual void ProgPostDecl(const Prog &prog) { }
};

class SymbolCollectionVisitor : public Visitor {
public: 
    SymbolCollectionVisitor() : Visitor() {}

    void ProgPostDecl(const Prog &prog) override {
        std::cout << "symbol collection prog post decl" << std::endl;
    }
};

class TreeTraveler : boost::static_visitor<> {
private: 
    Visitor &thing;

public:
    TreeTraveler(Visitor &thing) : thing(thing) {}

    template <typename T>
    void operator()(const T &t) {
        std::cout << "Visited unknown" << std::endl;
    }
};

template <>
void TreeTraveler::operator()(const Decl &decl) {
    //this->thing.Decl(decl);
}

template <>
void TreeTraveler::operator()(const Prog &prog) {
    this->thing.ProgPreDecl(prog);
    (*this)(prog.decl);
    this->thing.ProgPostDecl(prog);
}


Prog symbol_collection(Prog &prog) {
    auto thing = SymbolCollectionVisitor();
    auto visitor = TreeTraveler(thing);
    visitor(prog);
    return prog;
}

