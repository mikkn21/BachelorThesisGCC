#ifndef MGRAMMAR_COMPILER_ERROR_HPP
#define MGRAMMAR_COMPILER_ERROR_HPP 

#include "ast.hpp"

class CompilerError : public std::exception {
private:
    std::string message;
public:
    CompilerError(const std::string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
