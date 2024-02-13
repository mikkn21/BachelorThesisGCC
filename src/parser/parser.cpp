#include <boost/spirit/home/x3.hpp>
#include <sys/resource.h>
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
        const x3::rule<class prog, ast::Prog> prog = "prog";
        const x3::rule<class decl, ast::Decl> decl = "decl"; 
        const x3::rule<class var_decl, ast::VarDecl> var_decl = "var_decl"; 
        const x3::rule<class type, ast::Type> type = "type"; 
        const x3::rule<class id, ast::Id> id = "id"; 
        const x3::rule<class primitive_type, ast::PrimitiveType> primitive_type = "primitive_type"; 


        // Define a parser for operators
        const auto operator_parser = 
            x3::string("==") | x3::string("!=") | x3::string("<=") | x3::string(">=") |
            x3::string("+") | x3::string("-") | x3::string("*") | x3::string("/") |
            x3::string("%") | x3::string("<") | x3::string(">") | x3::string("&") | x3::string("|");

        const auto prog_def = decl;
        const auto decl_def = var_decl;
        const auto var_decl_def = type >> id >> "=" >> int_ >> ";";
        const auto type_def = primitive_type;
        const auto primitive_type_def = x3::string("int") | x3::string("bool");
        const auto id_def = (x3::char_("a-zA-Z_") >> *x3::char_("a-zA-Z_0-9"));
        const auto binop_exp_def = int_ >> operator_parser >> int_;   

        BOOST_SPIRIT_DEFINE(binop_exp); 

        bool parse(std::string src)
        {
            namespace x3 = boost::spirit::x3;
            using x3::ascii::space;
            using grammar::ast::Prog;
            using iterator_type = std::string::const_iterator;
           

            Prog obj;
            iterator_type iter = src.begin();
            iterator_type const end = src.end();
            bool r = phrase_parse(iter, end, prog, space, obj);

            return r;
        }
    } // namespace parser

} // namespace grammar


