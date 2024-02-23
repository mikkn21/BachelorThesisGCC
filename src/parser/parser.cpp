#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <sys/resource.h>
#include "../ast.hpp"
#include "parser.hpp"

using namespace std;


namespace grammar {
    namespace parser {
        
        // using iterator_type = std::string::const_iterator;
        // using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
        // BOOST_SPIRIT_INSTANTIATE(BinopExp_type, iterator_type, context_type);

        namespace x3 = boost::spirit::x3; 

        using x3::int_;
        using x3::bool_;
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
        const x3::rule<class function_call, ast::FunctionCall> function_call = "function_call";
        const x3::rule<class argument_list, ast::ArgumentList> argument_list = "argument_list";

        // Define a parser for operators
        const auto operator_parser = 
            x3::string("==") | x3::string("!=") | x3::string("<=") | x3::string(">=") |
            x3::string("+") | x3::string("-") | x3::string("*") | x3::string("/") |
            x3::string("%") | x3::string("<") | x3::string(">") | x3::string("&") | x3::string("|");
        
        // Useable
        const auto primitive_type_def = x3::string("int") | x3::string("bool");
        const auto type_def = primitive_type;  // | array_type;
        //const auto id_def = x3::lexeme[(x3::char_("a-zA-Z_") >> *x3::char_("a-zA-Z_0-9"))];
        auto const id_def = x3::raw[ x3::lexeme[(x3::char_("a-zA-Z_") >> *x3::char_("a-zA-Z_0-9"))]];
        const auto parameter_def = type >> id;
        const auto parameter_list_def = -(parameter % ','); // -(*(parameter >> ',') >> parameter);
        const auto expression_base = '(' >> expression >> ')' | int_ | bool_;
        const auto expression_def = binop_exp | expression_base; 
        const auto binop_exp_def = expression_base >> operator_parser >> expression;
        const auto block_line_def = decl;
        const auto block_def = '{' >> *block_line >> '}';
        const auto func_decl_def = type >> id >> '(' >> parameter_list >> ')' >> block;
        const auto var_decl_def = type >> id >> '=' >> expression >> ';';
        const auto decl_def = var_decl | func_decl;
        const auto prog_def = decl;

        // Not useable
        const auto array_type_def = type >> x3::lit("[]");
        const auto var_assign_def = id >> '=' >> expression >> ';';
        const auto while_statement_def = x3::lit("while") >> expression >> block;   
        const auto function_call_def = id >> '('>> argument_list >> ')'; 
        const auto argument_list_def = -(expression % ',');

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
            expression,
            function_call,
            argument_list
        )

        grammar::ast::Prog parse(std::string_view src)
        {
            namespace x3 = boost::spirit::x3;
            using x3::space;
            using grammar::ast::Prog;
         
            Prog ast;
            using PosIter = boost::spirit::line_pos_iterator<std::string_view::const_iterator>;
            PosIter iter(src.begin());

            const auto makeError = [&]() {
                // const auto lineNumber = iter.position();
                // const auto lineRange = get_current_line(PosIter(src.begin()), iter, PosIter(src.end()));
                // const auto column = get_column(lineRange.begin(), iter, SpacesPerTabs);
                // std::string msg = "Parsing failed at " + std::to_string(lineNumber) + ":" + std::to_string(column) + ":\n";
                // for(const char c : lineRange) {
                    // if(c == '\t') msg += std::string(SpacesPerTabs, ' ');
                    // else msg += c;
                // }
                std::string msg = "Parsing failed";
                msg += "\n";
                // msg += std::string(column - 1, '-');
                // msg += "^";
                return msg;
	        };

            try {
                bool r = phrase_parse(iter,  PosIter(src.end()), prog, space, ast);
                
                if(!r || iter !=  PosIter(src.end())) {
                    throw SyntaxError(makeError() + "\nEnd of x3 error.");
                }
		        return ast;

            }catch(const x3::expectation_failure<PosIter> &e) {
	            iter = e.where();
		        throw SyntaxError(makeError()
		            + "\nExpected " + e.which() + ".\n"
		            + "End of x3 error.");
            }
        }
    } // namespace parser

} // namespace grammar


