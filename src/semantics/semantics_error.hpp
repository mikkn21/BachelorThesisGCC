
#ifndef MGRAMMAR_SEMANTICS_ERROR_HPP
#define MGRAMMAR_SEMANTICS_ERROR_HPP 

#include "../error/ast_error.hpp"


class SemanticsError : public ASTError {
public:
    template <typename T>
    SemanticsError(const std::string& msg, const T& node)
        : ASTError(msg, node, "SemanticsError") {}
    };

#endif
