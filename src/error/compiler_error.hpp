#ifndef MY_ERROR_HPP
#define MY_ERROR_HPP 


#include <string>


class CompilerError : public std::runtime_error {
private:
public:
    CompilerError(const std::string& msg) : std::runtime_error(msg) {}

};

#endif
