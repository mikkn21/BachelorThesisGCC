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
        std::ostream& operator<<(std::ostream& os, const grammar::ast::BinopExp &exp) {
            return os << exp.lhs << " " << exp.op << " " << exp.rhs;
        }


        std::ostream& operator<<(std::ostream& os, const grammar::ast::Id &id) {
            return os << id.id;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::PrimitiveType &type) {
            return os << type.type;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Expression &exp) {
            boost::apply_visitor(print_visitor(os), exp);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::BlockLine &block_line) {
            boost::apply_visitor(print_visitor(os), block_line);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Block &block) {
            for (const auto &i : block.block_line) os << i << std::endl; 
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Type &type) {
            return os << type.primitive_type;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::VarDecl &decl) {
            return os << decl.type << " " << decl.id << " = " << decl.exp << ";";
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Parameter &parameter) {
            return os << parameter.type << " " << parameter.id;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::ParameterList &input) {
            if (input.parameter.empty()) {
                return os;
            }

            const auto parameters = input.parameter;
            os << parameters[0];
            for (unsigned long i = 0; i < parameters.size(); i++) {
                os << ", ";
                os << parameters[i];
            }
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::FuncDecl &func) {
            return os << func.type << " " << func.id << " (" << func.parameter_list << ") " << func.block;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::ArrayType &arr) {
            return os << arr.type << "[]";
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::VarAssign &assign) {
            return os << assign.id << " = " << assign.exp;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::WhileStatement &while_statement) {
            return os << "while " << while_statement.exp << " " << while_statement.block;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Decl &decl) {
            boost::apply_visitor(print_visitor(os), decl);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Prog &prog) {
            return os << prog.decl;
        }
    } // ast
}; // grammar
