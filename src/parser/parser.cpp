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
        const x3::rule<class id, ast::Id> id = "id"; 
        const x3::rule<class primitive_type, ast::PrimitiveType> primitive_type = "primitive_type"; 
        const x3::rule<class block_line, ast::BlockLine> block_line = "block_line"; 
        const x3::rule<class block, ast::Block> block = "block"; 
        const x3::rule<class type, ast::Type> type = "type"; 
        const x3::rule<class var_decl, ast::VarDecl> var_decl = "var_decl"; 
        const x3::rule<class parameter, ast::Parameter> parameter = "parameter"; 
        const x3::rule<class parameter_list, ast::ParameterList> parameter_list = "parameter_list"; 
        const x3::rule<class func_decl, ast::FuncDecl> func_decl = "func_decl"; 
        const x3::rule<class array_type, ast::ArrayType> array_type = "array_type"; 
        const x3::rule<class var_assign, ast::VarAssign> var_assign = "var_assign"; 
        const x3::rule<class while_statement, ast::WhileStatement> while_statement = "while_statement"; 
        const x3::rule<class decl, ast::Decl> decl = "decl"; 
        const x3::rule<class prog, ast::Prog> prog = "prog";
        const x3::rule<class expression,  ast::Expression> expression = "expression";


        // Define a parser for operators
        const auto operator_parser = 
            x3::string("==") | x3::string("!=") | x3::string("<=") | x3::string(">=") |
            x3::string("+") | x3::string("-") | x3::string("*") | x3::string("/") |
            x3::string("%") | x3::string("<") | x3::string(">") | x3::string("&") | x3::string("|");

        const auto binop_exp_def = int_ >> operator_parser >> int_;   
        const auto id_def = x3::raw[(x3::char_("a-zA-Z_") >> *x3::char_("a-zA-Z_0-9"))];
        const auto primitive_type_def = x3::string("int") | x3::string("bool");
        const auto block_line_def = decl;
        const auto block_def = '{' >> *block_line >> '}';
        const auto type_def = primitive_type;  // | array_type;
        const auto var_decl_def = type >> id >> '=' >> expression >> ';';
        const auto parameter_def = type >> id;
        const auto parameter_list_def = -(parameter % ','); // -(*(parameter >> ',') >> parameter);
        const auto func_decl_def = type >> id >> '(' >> parameter_list >> ')' >> block;
        const auto array_type_def = type >> x3::string("[]");
        const auto var_assign_def = id >> '=' >> expression >> ';';
        const auto while_statement_def = x3::string("while") >> expression >> block;
        const auto decl_def = var_decl | func_decl;
        const auto prog_def = decl;
        const auto expression_def = '(' >> expression >> ')' | binop_exp | int_; 
        

        BOOST_SPIRIT_DEFINE(
            binop_exp,
            id,
            primitive_type,
            block_line,
            block,
            type,
            var_decl,
            parameter,
            parameter_list,
            func_decl,
            array_type,
            var_assign,
            while_statement,
            decl,
            prog,
            expression
        )

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


