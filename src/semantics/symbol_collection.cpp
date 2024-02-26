
#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"
#include "../visitor.hpp"

class SymbolCollectionVisitor : public Visitor {
public: 
    SymbolCollectionVisitor() : Visitor() {}

    void ProgPostDecl(const Prog &prog) override {
        std::cout << "symbol collection prog post dec" << std::endl;
    }
};

Prog symbol_collection(Prog &prog) {
    auto thing = SymbolCollectionVisitor();
    auto visitor = TreeTraveler(thing);
    visitor(prog);
    return prog;
}

