#ifndef MGRAMMAR_PARSER_HPP
#define MGRAMMAR_PARSER_HPP


#include <boost/spirit/home/x3/nonterminal.hpp>


#include "../ast.hpp"

#include "../error/compiler_error.hpp"


namespace grammar {

    namespace parser {

    
    grammar::ast::Prog parse(std::string_view src);
    extern const boost::spirit::x3::rule<class prog, ast::Prog> prog;

    class SyntaxError : public CompilerError {
    public:
        using CompilerError::CompilerError;
    };
    
    bool isReserved(const std::string &keyword);
        
    } // namespace parser 

} // namespace grammar 




#endif
