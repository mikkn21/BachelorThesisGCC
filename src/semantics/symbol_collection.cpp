
#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"

using boost::apply_visitor;

class SymbolCollectionVisitor : boost::static_visitor<> {
public: 
    SymbolCollectionVisitor() {}

    template <typename T>
    void operator()(const T &t) {
        std::cout << "Visited unknown" << std::endl;
    }
};

template <>
void SymbolCollectionVisitor::operator()(const Decl &decl) {
    std::cout << "Visited decl" << std::endl;
}

template <>
void SymbolCollectionVisitor::operator()(const Prog &prog) {
    std::cout << "Visited prog" << std::endl;
    (*this)(prog.decl);
}


Prog symbol_collection(Prog &prog) {
    auto visitor = SymbolCollectionVisitor();
    visitor(prog);
    return prog;
}

