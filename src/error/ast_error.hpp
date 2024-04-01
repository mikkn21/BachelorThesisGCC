#ifndef AST_ERROR_HPP
#define AST_ERROR_HPP 
#include "compiler_error.hpp"
#include <optional>
#include <string>
#include "../ast.hpp"
#include <sstream>
#include <type_traits>

using namespace std;

class ASTError : public CompilerError {

public:
    template <typename T>
    ASTError(const string& msg, const T& node, const string& errorType)
        : CompilerError(buildErrorMessage(msg, node, errorType)) {}

    ASTError(const string& msg, const string& errorType)
        : CompilerError(errorType + ": " +  msg + "\n") {}

private: 
    template <typename T>
    static string buildErrorMessage(const string& msg, const T& node, const string& errorType) {
      static_assert(is_base_of<LocationInfo, T>::value,"T must be a subclass of LocationInfo");
      ostringstream oss;
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