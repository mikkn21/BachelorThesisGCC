#include "ir.hpp"

using namespace std;
void emit(IR ir);

class EmitError: public CompilerError {
public: 
    using CompilerError::CompilerError;
};