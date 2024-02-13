
#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

using namespace std;


namespace grammar {
    namespace parser {
        
        // using iterator_type = std::string::const_iterator;
        // using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
        // BOOST_SPIRIT_INSTANTIATE(BinopExp_type, iterator_type, context_type);

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


        bool parse(std::string src)
        {
            namespace x3 = boost::spirit::x3;
            using x3::ascii::space;
            using grammar::ast::BinopExp;
            using iterator_type = std::string::const_iterator;
           

            BinopExp obj;
            iterator_type iter = src.begin();
            iterator_type const end = src.end();
            bool r = phrase_parse(iter, end, binop_exp, space, obj);

            return r;
        }
    } // namespace parser

} // namespace grammar


