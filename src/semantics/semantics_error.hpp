
#ifndef MGRAMMAR_SEMANTICS_ERROR_HPP
#define MGRAMMAR_SEMANTICS_ERROR_HPP 

#include "../error/compiler_error.hpp"

class SemanticsError : public CompilerError {
public:
    using CompilerError::CompilerError;
};

#endif
