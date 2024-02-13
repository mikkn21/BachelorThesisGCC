#include "ast.hpp"
#include "grammar.hpp"

using namespace std;


namespace grammar {
    namespace parser {
        
        // using iterator_type = std::string::const_iterator;
        // using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
        // BOOST_SPIRIT_INSTANTIATE(BinopExp_type, iterator_type, context_type);


        bool parse(std::string src)
        {
            namespace x3 = boost::spirit::x3;
            using x3::ascii::space;
            using grammar::ast::BinopExp;
            using iterator_type = std::string::const_iterator;
           

            BinopExp obj;
            iterator_type iter = src.begin();
            iterator_type const end = src.end();
            bool r = phrase_parse(iter, end, grammar::BinopExp(), space, obj);

            return r;
        }
    } // namespace parser

} // namespace grammar


