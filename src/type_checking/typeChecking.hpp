#ifndef TYPE_CHECK_ERROR
#define TYPE_CHECK_ERROR

#include "../error/base_error.hpp"



    class TypeCheckError: public BaseError {
    public:
        using BaseError::BaseError;
    };

#endif //TYPE_CHECK_ERROR 
