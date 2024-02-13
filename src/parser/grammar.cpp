#include "grammar.hpp"
#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"


namespace grammar 
{
    namespace parser 
    {
        namespace x3 = boost::spirit::x3; 

        using x3::int_;
        using namespace grammar::ast;

        // Rules up here:
        const x3::rule<class binop_exp, ast::BinopExp> binop_exp = "binop_exp"; 


        // Define a parser for operators
        const auto operator_parser = 
            x3::string("==") | x3::string("!=") | x3::string("<=") | x3::string(">=") |
            x3::string("+") | x3::string("-") | x3::string("*") | x3::string("/") |
            x3::string("%") | x3::string("<") | x3::string(">") | x3::string("&") | x3::string("|");

        const auto binop_exp_def = int_ >> operator_parser >> int_;   

        BOOST_SPIRIT_DEFINE(binop_exp); 

    } // namespace parser 
    
    parser::BinopExp_type BinopExp() 
    {
        return parser::binop_exp;
    }

} // namespace grammar

