#include <iostream>
#include "ast.hpp"
namespace {
    struct print_visitor : boost::static_visitor<> {
        std::ostream& os;
        print_visitor(std::ostream& os) : os(os) {}

        template<typename T>
        void operator()(const T& t) const {
            os << t;
        }
    };
} //namespace

namespace grammar 
{ 
    namespace ast
    {   
        //Idea for handling variants: simply make checks for each type of variants
        std::ostream& operator<<(std::ostream& os, const grammar::ast::BinopExp &input) {
            return os << input.lhs << " " << input.op << " " << input.rhs;
        }


        std::ostream& operator<<(std::ostream& os, const grammar::ast::Id &input) {
            return os << input.id;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::PrimitiveType &input) {
            return os << input.type;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Expression &input) {
            boost::apply_visitor(print_visitor(os), input);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::BlockLine &input) {
            boost::apply_visitor(print_visitor(os), input);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Block &input) {
            for (const auto &i : input.block_line) os << i << " "; 
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Type &input) {
            return os << input.primitive_type;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::VarDecl &input) {
            return os << input.type << " " << input.id << " " << input.exp;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Parameter &input) {
            os << ""; 
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::ParameterList &input) {
            os << ""; 
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::FuncDecl &input) {
            //os << exp.type << " " << exp.id << " " << exp.parameter_list << " " << exp.block;
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::ArrayType &input) {
            os << ""; 
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::VarAssign &input) {
            os << ""; 
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::WhileStatement &input) {
            os << ""; 
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Decl &input) {
            boost::apply_visitor(print_visitor(os), input);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Prog &input) {
            os << input.decl;
            return os;
        }
    } // ast
}; // grammar