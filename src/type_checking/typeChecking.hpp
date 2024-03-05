#ifndef TYPE_CHECK_ERROR
#define TYPE_CHECK_ERROR

#include "../error/base_error.hpp"

    class TypeCheckError: public BaseError {
    public:
        using BaseError::BaseError;
    };
    
    template<typename type>
    void typeCheckHelper(type);

#endif //TYPE_CHECK_ERROR 
