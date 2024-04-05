#ifndef AST_ERROR_HPP
#define AST_ERROR_HPP 

#include "../ast.hpp"
#include "compiler_error.hpp"
#include <sstream>




class ASTError : public CompilerError {

public:
    template <typename T>
    ASTError(const std::string& msg, const T& node, const std::string& errorType)
        : CompilerError(buildErrorMessage(msg, node, errorType)) {}

    ASTError(const std::string& msg, const std::string& errorType)
        : CompilerError(errorType + ": " +  msg + "\n") {}

private: 

    template <typename T>
    static std::string buildErrorMessage(const std::string& msg, const T& node, const std::string& errorType) {
      static_assert(std::is_base_of<grammar::ast::LocationInfo, T>::value,"T must be a subclass of LocationInfo");
      std::ostringstream oss;
      oss << node;
      const auto lineNumber = node.line;
      const auto column = node.column;
      std::string errorMessage = errorType + ": " + msg + "\n";
      errorMessage += "Error found at " + std::to_string(lineNumber) + ":" +
                      std::to_string(column) + " " + oss.str();

      return errorMessage;
    }
};

#endif