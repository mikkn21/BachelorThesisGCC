
#include "symbol_collector.hpp"
#include <iostream>

void SymbolCollectionVisitor::visitProgPreDecl(const Prog &prog) {
    std::cout << "Pre-visit decl for prog" << std::endl;
}
