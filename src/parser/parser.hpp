#ifndef MGRAMMAR_PARSER_HPP
#define MGRAMMAR_PARSER_HPP

#include <string>
#include <exception>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <sys/resource.h>
#include "ast.hpp"
#include "parser.hpp"

namespace grammar {
    namespace parser {

    
    grammar::ast::Prog parse(std::string_view src);


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