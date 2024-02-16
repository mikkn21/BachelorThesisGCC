#include <iostream>
#include "ast.hpp"

std::ostream& operator<<(std::ostream& os, const grammar::ast::PrimitiveType exp) {
    os << exp.type;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Id exp) {
    os << exp.id;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Type exp) {
    os << exp.primitive_type;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::VarDecl exp) {
    os << exp.type << " " << exp.id << " " << exp.exp;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Decl exp) {
    os << exp.var_decl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Prog exp) {
    os << exp.decl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::BinopExp exp) {
    os << exp.lhs << " " << exp.op << " " << exp.rhs;
    return os;
}