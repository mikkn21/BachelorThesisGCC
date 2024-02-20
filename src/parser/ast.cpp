#include <iostream>
#include "ast.hpp"

struct print_visitor : boost::static_visitor<> {
    std::ostream& os;
    print_visitor(std::ostream& os) : os(os) {}

    template<typename T>
    void operator()(const T& t) const {
        os << t; // This will call the appropriate operator<< based on T
    }
};


//Idea for handling variants: simply make checks for each type of variants

std::ostream& operator<<(std::ostream& os, const grammar::ast::BinopExp exp) {
    os << exp.lhs << " " << exp.op << " " << exp.rhs;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Id exp) {
    os << exp.id;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::PrimitiveType exp) {
    os << exp.type;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Expression exp) {
    boost::apply_visitor(print_visitor(os), exp);
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::BlockLine exp) {
    os << exp; 
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Block exp) {
    for (grammar::ast::BlockLine i : exp.block_line) os << i << " "; 
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

std::ostream& operator<<(std::ostream& os, const grammar::ast::Parameter exp) {
    os << ""; 
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::ParameterList exp) {
    os << ""; 
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::FuncDecl exp) {
    os << exp.type << " " << exp.id << " " << exp.parameter_list << " " << exp.block;
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::ArrayType exp) {
    os << ""; 
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::VarAssign exp) {
    os << ""; 
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::WhileStatement exp) {
    os << ""; 
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Decl exp) {
    boost::apply_visitor(print_visitor(os), exp);
    return os;
}

std::ostream& operator<<(std::ostream& os, const grammar::ast::Prog exp) {
    os << exp.decl;
    return os;
}