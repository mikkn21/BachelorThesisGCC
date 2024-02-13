#include <iostream>
#include "ast.hpp"

std::ostream& operator<<(std::ostream& os, const grammar::ast::BinopExp& exp) {
    os << exp.lhs << " " << exp.op << " " << exp.rhs;
    return os;
}