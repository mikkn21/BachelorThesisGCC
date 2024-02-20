#ifndef MGRAMMAR_PARSER_HPP
#define MGRAMMAR_PARSER_HPP

#include <string>
#include <exception>

namespace grammar {
    namespace parser {

    
    bool parse(std::string_view src);


    class SyntaxError : public std::exception {
    private:
        std::string message;
    public:
        SyntaxError(const std::string& msg) : message(msg) {}

        virtual const char* what() const noexcept override {
            return message.c_str();
        }
    };

        
    } // namespace parser 

} // namespace grammar 




#endif