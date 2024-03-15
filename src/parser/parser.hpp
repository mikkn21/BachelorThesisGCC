#ifndef MGRAMMAR_PARSER_HPP
#define MGRAMMAR_PARSER_HPP

#include <string>
#include "../ast.hpp"
#include <boost/spirit/home/x3.hpp>
#include "../error/base_error.hpp"


namespace grammar {

    namespace parser {

    
    grammar::ast::Prog parse(std::string_view src);
    extern const x3::rule<class prog, ast::Prog> prog;

    class SyntaxError : public BaseError {
    public:
        using BaseError::BaseError;
    };
    
    bool isReserved(const std::string &keyword);
        
    } // namespace parser 

} // namespace grammar 




#endif
