#ifndef TYPE_CHECK_ERROR
#define TYPE_CHECK_ERROR

#include "../error/base_error.hpp"
#include "../ast.hpp"

    class TypeCheckError: public BaseError {
    public:
        using BaseError::BaseError;
    };

    Prog typeChecker(Prog &prog);

#endif //TYPE_CHECK_ERROR 
