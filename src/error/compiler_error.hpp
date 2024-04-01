#ifndef MY_ERROR_HPP
#define MY_ERROR_HPP 
#include <stdexcept>
#include <string>


class CompilerError : public std::runtime_error {
private:
    // std::string message;
public:
    CompilerError(const std::string& msg) : std::runtime_error(msg) {}

    // virtual const char* what() const noexcept override {
    //     return message.c_str();
    // }
};

#endif
