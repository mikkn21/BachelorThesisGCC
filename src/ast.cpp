#include <iostream>
#include "ast.hpp"
#include "semantics/symbol_table.hpp"

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
        FuncDecl::FuncDecl() = default;
        FuncDecl::~FuncDecl() = default;
        
        std::ostream& operator<<(std::ostream& os, const grammar::ast::LocationInfo &location_info) {
            return os << '[' << location_info.line << ", " << location_info.column << ']';
        };
        
        std::ostream& operator<<(std::ostream& os, const grammar::ast::BinopExps &exp) {
            os << exp.lhs; 
            for (unsigned long i = 0; i < exp.rhss.size(); i++) {
                os << " " << exp.rhss[i];
            }
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Rhs &rhs) {
            return os << rhs.op << " " << rhs.exp;
        }

        std::ostream& operator<<(std::ostream& os, const IfStatement &exp) {
            return os << "if " << exp.exp << " " << exp.block;
        }

        std::ostream& operator<<(std::ostream& os, const ElseStatement &els) {
            return os << "else " << els.block;
        }

        std::ostream& operator<<(std::ostream& os, const ConditionalStatement &exp) {
            os << exp.ifStatement;
            const auto ifs = exp.elseIfs;
            for (unsigned long i = 0; i < ifs.size(); i++) {
                os << "else ";
                os << ifs[i];
            }
            if (exp.conditionalElse.has_value()) {
                os << exp.conditionalElse.value();
            }
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const VarExpression &id) {
            return os << id.id;
        }

        std::ostream& operator<<(std::ostream& os, const FunctionCall &funcCall) {
            return os << funcCall.id << "(" << funcCall.argument_list << ")";
        }

        std::ostream& operator<<(std::ostream& os, const ArgumentList &argument) {
            if (argument.arguments.empty()) {
                return os;
            }

            const auto arguments = argument.arguments;
            os << arguments[0];
            for (unsigned long i = 1; i < arguments.size(); i++) {
                os << ", ";
                os << arguments[i];
            }
            return os;
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

        std::ostream& operator<<(std::ostream& os, const grammar::ast::ReturnStatement &ret) {
            return os << "return " << ret.exp << ";";
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
            return os << decl.type << " " << decl.id;
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::VarDeclAssign &decl) {
            return os << decl.decl << " = " << decl.exp << ";\n";
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::VarDeclStatement &decl) {
            boost::apply_visitor(print_visitor(os), decl);
            return os << ";\n";
        }

        std::ostream& operator<<(std::ostream& os, const grammar::ast::Parameter &parameter) {
            boost::apply_visitor(print_visitor(os), parameter);
            return os;
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
