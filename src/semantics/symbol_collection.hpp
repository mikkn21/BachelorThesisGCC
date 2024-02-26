
#ifndef MGRAMMAR_SYMBOL_COLLECTION_HPP
#define MGRAMMAR_SYMBOL_COLLECTION_HPP 

#include "../ast.hpp"

Prog symbol_collection(Prog &prog);

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