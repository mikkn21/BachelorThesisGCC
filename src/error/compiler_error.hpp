#ifndef MY_ERROR_HPP
#define MY_ERROR_HPP 
#include <string>
#include "../ast.hpp"
#include <sstream>
#include <optional>

class CompilerError : public std::exception {
private:
    std::string message;
    std::optional<LocationInfo> location_info;
    mutable std::string full_message;

public:
    CompilerError(std::string message, std::optional<LocationInfo> location_info = std::nullopt)
        : message(std::move(message)), location_info(std::move(location_info)) {}

    virtual const char* what() const noexcept override {
        std::ostringstream oss;
        if (location_info) {
            oss << location_info.value() << ": " << message;
        } else {
            oss << message;
        }
        full_message = oss.str(); 
        return full_message.c_str();
    }
};

#endif
