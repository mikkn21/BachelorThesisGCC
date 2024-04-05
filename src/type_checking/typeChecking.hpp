#ifndef TYPE_CHECK_HPP
#define TYPE_CHECK_HPP

#include "../error/ast_error.hpp"
    
    class TypeCheckError: public ASTError {
    public:
        template <typename T>
        TypeCheckError(const std::string& msg, const T& node)
            : ASTError(msg, node, "TypeCheckError") {}
        TypeCheckError(const std::string& msg)
            : ASTError(msg, "TypeCheckError") {}
    };

    class EmptyTypeStackError : public CompilerError {
    public:
        EmptyTypeStackError()
            : CompilerError("Attempting to pop from an empty stack") {}
    };

    grammar::ast::Prog typeChecker(grammar::ast::Prog &prog, SymbolTable *globalScope);


#endif //TYPE_CHECK_HPP
