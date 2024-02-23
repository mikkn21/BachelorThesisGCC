
#include <iostream>
#include <boost/variant.hpp>
#include "../ast.hpp"

using boost::apply_visitor;

class SymbolCollectionVisitor : boost::static_visitor<> {
public: 
    SymbolCollectionVisitor() {}

    template <typename T>
    void operator()(const T &t) const {
        std::cout << "Visited unknown" << std::endl;
    }

    void operator()(const Prog &prog) const {
        std::cout << "Visited prog" << std::endl;
        apply_visitor(this, prog.decl);
    }
};

Prog symbol_collection(Prog &prog) {
    auto const visitor = SymbolCollectionVisitor();
    apply_visitor(visitor, prog);
    return prog;
}

