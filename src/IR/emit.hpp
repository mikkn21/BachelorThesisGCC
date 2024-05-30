#include "ir.hpp"

using namespace std;
void emit(IR &ir, string output_file);

class EmitError: public CompilerError {
public: 
    using CompilerError::CompilerError;
};