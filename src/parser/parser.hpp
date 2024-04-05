#ifndef MGRAMMAR_PARSER_HPP
#define MGRAMMAR_PARSER_HPP

// !! is_variant and tuple_traits needs to be here for nonterminal to work!!!
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#include <boost/spirit/home/x3/support/traits/tuple_traits.hpp>
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
