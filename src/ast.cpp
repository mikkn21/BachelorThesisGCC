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
        void operator()(bool t) const {
            os << (t ? "true" : "false");;
        }
    };
} //namespace

namespace grammar 
{ 
    namespace ast
    {   
        
        std::ostream& operator<<(std::ostream& os, const grammar::ast::LocationInfo &location_info) {
            return os << '[' << location_info.line << ", " << location_info.column << ']';
        };
        
        std::ostream& operator<<(std::ostream& os, const grammar::ast::BinopExp &exp) {
            return os << exp.lhs << " " << exp.op << " " << exp.rhs;
        }


        std::ostream& operator<<(std::ostream& os, const grammar::ast::Id &id) {
            return os << id.id;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::PrimitiveType &type) {
            return os << type.type;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::StatementExpression &exp) {
            return os << exp.exp << ';' << "\n";
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Expression &exp) {
            boost::apply_visitor(print_visitor(os), exp);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::ExpressionPar &exp) {
            return os << "(" << exp.exp << ")";
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::PrintStatement &exp) {
            return os << "print(" << exp.exp << ");";
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::BlockLine &block_line) {
            boost::apply_visitor(print_visitor(os), block_line);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Block &block) {
            os << "{\n";
            for (const auto &i : block.block_line) {
                os << i; 
            }
            os << "}\n";
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Type &type) {
            boost::apply_visitor(print_visitor(os), type);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::VarDecl &decl) {
            return os << decl.type << " " << decl.id << " = " << decl.exp << ";\n";
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
            for (unsigned long i = 1; i < parameters.size(); i++) {
                os << ", ";
                os << parameters[i];
            }
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Statement &statement) {
            boost::apply_visitor(print_visitor(os), statement);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::FuncDecl &func) {
            return os << func.type << " " << func.id << " (" << func.parameter_list << ") " << func.block;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::ArrayType &arr) {
            return os << arr.type << "[]";
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::VarAssign &assign) {
            return os << assign.id << " = " << assign.exp << ';' << "\n" ;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::WhileStatement &while_statement) {
            return os << "while " << while_statement.exp << " " << while_statement.block;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Decl &decl) {
            boost::apply_visitor(print_visitor(os), decl);
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Prog &prog) {
            for (const auto &decl : prog.decls) {
                os << decl;
            }
            return os;
        }
    } // ast
}; // grammar
