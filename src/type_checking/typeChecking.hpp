#ifndef TYPE_CHECK_HPP
#define TYPE_CHECK_HPP

#include "../error/base_error.hpp"
#include "../ast.hpp"

    class TypeCheckError: public BaseError {
    public:
        using BaseError::BaseError;
    };

    Prog typeChecker(Prog &prog);


#endif //TYPE_CHECK_HPP
