#ifndef MY_ERROR_HPP
#define MY_ERROR_HPP 
#include <string>


class BaseError : std::exception {
private:
    std::string message;
public:
    BaseError(const std::string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
