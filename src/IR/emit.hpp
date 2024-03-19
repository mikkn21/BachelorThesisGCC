#include "ir.hpp"

void emit(IR ir);

class EmitError: public CompilerError {
public: 
    using CompilerError::CompilerError;
};