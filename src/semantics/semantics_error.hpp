
#ifndef MGRAMMAR_SEMANTICS_ERROR_HPP
#define MGRAMMAR_SEMANTICS_ERROR_HPP 

#include "../ast.hpp"

class SemanticsError : public std::exception {
private:
    std::string message;
public:
    SemanticsError(const std::string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
