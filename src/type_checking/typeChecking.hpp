#ifndef TYPE_CHECK_HPP
#define TYPE_CHECK_HPP

#include "../error/base_error.hpp"
#include "../ast.hpp"

    class TypeCheckError: public CompilerError {
    public:
        using CompilerError::CompilerError;
    };

    Prog typeChecker(Prog &prog, SymbolTable *globalScope);


#endif //TYPE_CHECK_HPP
