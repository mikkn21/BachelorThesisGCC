#ifndef AST_ERROR_HPP
#define AST_ERROR_HPP 

#include "../ast.hpp"
#include "compiler_error.hpp"
#include <sstream>




class ASTError : public CompilerError {

public:
    template <typename T>
    ASTError(const std::string& msg, const T& node, const std::string& error_type)
        : CompilerError(build_error_message(msg, node, error_type)) {}

    ASTError(const std::string& msg, const std::string& error_type)
        : CompilerError(error_type + ": " +  msg + "\n") {}

private: 

    template <typename T>
    static std::string build_error_message(const std::string& msg, const T& node, const std::string& error_type) {
        static_assert(std::is_base_of<grammar::ast::LocationInfo, T>::value,"T must be a subclass of LocationInfo");
        std::ostringstream oss;
        oss << node;
        const auto line_number = node.line;
        std::string error_message = error_type + ": " + msg + "\n";
        error_message += "Error found at line " + std::to_string(line_number) + ": " + oss.str();

        return error_message;
    }
};

#endif