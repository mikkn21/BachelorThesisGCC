
#include "visitor.hpp"

template <>
void TreeTraveler::operator()(const Decl &decl) {
    //this->visitor.Decl(decl);
}

template <>
void TreeTraveler::operator()(const Prog &prog) {
    this->visitor.ProgPreDecl(prog);
    (*this)(prog.decl);
    this->visitor.ProgPostDecl(prog);
}
