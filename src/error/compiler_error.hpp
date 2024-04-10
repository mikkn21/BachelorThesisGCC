#ifndef MY_ERROR_HPP
#define MY_ERROR_HPP 


#include <string>
#include "../ast.hpp"
#include <sstream>
#include <optional>
#include <stdexcept>

class CompilerError : public std::runtime_error {
public:
    CompilerError(const std::string& msg) : std::runtime_error(msg) {}
};

#endif
