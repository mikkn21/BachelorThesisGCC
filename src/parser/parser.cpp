// #define BOOST_SPIRIT_X3_DEBUG

#include "parser.hpp"
#include "../ast_adapt.hpp" 

#include <boost/spirit/include/support_line_pos_iterator.hpp>

#include <boost/spirit/home/support/char_encoding/iso8859_1.hpp>
#include <boost/spirit/home/x3/string.hpp>
#include <boost/spirit/home/x3/operator.hpp>
#include <boost/spirit/home/x3/directive/lexeme.hpp>
#include <boost/spirit/home/x3/directive/raw.hpp>
#include <boost/spirit/home/x3/char/char.hpp>
#include <boost/spirit/home/x3/numeric/bool.hpp>
#include <boost/spirit/home/x3/numeric/int.hpp>
#include <boost/spirit/home/x3/char/char_class.hpp>

#include <boost/spirit/home/x3/core/parse.hpp>

#include <sys/resource.h>

// #include <boost/spirit/home/x3/string/symbols.hpp> 





namespace grammar {
    namespace parser {

        namespace x3 = boost::spirit::x3;
    
        namespace {
            constexpr int SpacesPerTabs = 4;   
        } // namespace

        struct LocationHandler {
	    template<typename Iterator, typename Context>
	    void on_success(const Iterator &first, const Iterator &last, ast::LocationInfo &locInfo, const Context &) {
		    locInfo.line = get_line(first);
		    locInfo.column = get_column(first, last, SpacesPerTabs);
	       }
        };


        // Rules up here:
        const x3::rule<class factor_rule, ast::BinopExps>  factor = "factor";      
        const x3::rule<class term_rule, ast::BinopExps>  term = "term";
        const x3::rule<class comp_rule, ast::BinopExps>  comp = "comp";      
        const x3::rule<class eq_rule, ast::BinopExps>  eq = "eq";
        const x3::rule<class logical_and_rule, ast::BinopExps>  logical_and = "logical_and";
        const x3::rule<class logical_or_rule, ast::BinopExps>  logical_or = "logical_or";

        const x3::rule<class factor_rule_rhs, ast::Rhs>  factor_rhs = "factor_rhs";      
        const x3::rule<class term_rule_rhs, ast::Rhs>  term_rhs = "term_rhs";
        const x3::rule<class comp_rule_rhs, ast::Rhs>  comp_rhs = "comp_rhs";      
        const x3::rule<class eq_rule_rhs, ast::Rhs>  eq_rhs = "eq_rhs";
        const x3::rule<class logical_and_rule_rhs, ast::Rhs>  logical_and_rhs = "logical_and_rhs";
        const x3::rule<class logical_or_rule_rhs, ast::Rhs>  logical_or_rhs = "logical_or_rhs";
      

        const x3::rule<class id, ast::Id> id = "id";
        const x3::rule<class primitive_type, ast::PrimitiveType> primitive_type = "primitive_type";
        const x3::rule<class block_line, ast::BlockLine> block_line = "block_line";
        const x3::rule<class block, ast::Block> block = "block";
        const x3::rule<class type, ast::Type> type = "type";
        const x3::rule<class var_decl, ast::VarDecl> var_decl = "var_decl";
        const x3::rule<class var_decl_assign, ast::VarDeclAssign> var_decl_assign = "var_decl_assign";
        const x3::rule<class var_decl_statement, ast::VarDeclStatement> var_decl_statement = "var_decl_statement";
        const x3::rule<class parameter, ast::Parameter> parameter = "parameter";
        const x3::rule<class parameter_list, ast::ParameterList> parameter_list = "parameter_list";
        const x3::rule<class func_decl, ast::FuncDecl> func_decl = "func_decl";
        const x3::rule<class array_type, ast::ArrayType> array_type = "array_type";
        const x3::rule<class var_assign, ast::VarAssign> var_assign = "var_assign";
        const x3::rule<class while_statement, ast::WhileStatement> while_statement = "while_statement";
        const x3::rule<class decl, ast::Decl> decl = "decl";
        const x3::rule<class prog, ast::Prog> prog = "prog";
        const x3::rule<class expression,  ast::Expression> expression = "expression";
        const x3::rule<class expression_par, ast::ExpressionPar> expression_par = "expression_par";
        const x3::rule<class function_call, ast::FunctionCall> function_call = "function_call";
        const x3::rule<class argument_list, ast::ArgumentList> argument_list = "argument_list";
        const x3::rule<class statement_expression, ast::StatementExpression> statement_expression = "statement_expression";
        const x3::rule<class statement, ast::Statement> statement = "statement";
        const x3::rule<class print_statement, ast::PrintStatement> print_statement = "print_statement";
        const x3::rule<class return_statement, ast::ReturnStatement> return_statement = "return_statement";
        const x3::rule<class var_expression, ast::VarExpression> var_expression =  "var_expression";

        const x3::rule<class if_statement, ast::IfStatement> if_statement =  "if_statement";
        const x3::rule<class else_statement, ast::ElseStatement> else_statement =  "else_statement";
        const x3::rule<class conditional_statement, ast::ConditionalStatement> conditional_statement =  "conditional_statement";

        

        struct reservedkeywords : x3::symbols<std::string> {
            reservedkeywords() {
                add("if", "if")
                    ("else", "else")
                    ("while", "while")
                    ("return", "return")
                    ("int", "int")
                    ("bool", "bool")
                    ("true", "true")
                    ("false", "false")
                    ("print", "print"); 
            }
        } reservedkeywordsInstance; 

        
        // highest to lowset precedence
        const auto mul_op = x3::string("*") | x3::string("/") | x3::string("%");
        const auto add_op = x3::string("+") | x3::string("-");
        const auto comp_op = x3::string("<=") | x3::string(">=") | x3::string("<") | x3::string(">");
        const auto eq_op = x3::string("==") | x3::string("!=");
        const auto logical_and_op = x3::string("&");
        const auto logical_or_op = x3::string("|");

        auto optionalParExp(std::string s) {
            return x3::lit(s + " ") > expression | x3::lit(s) > expression_par;
        }


        // Useable
        const auto primitive_type_def = x3::string("int") | x3::string("bool");
        const auto type_def = primitive_type;  // | array_type;
        const auto id_def = x3::raw[ x3::lexeme[( x3::char_("a-zA-Z_") >> * boost::spirit::x3::char_("a-zA-Z_0-9"))]] - (reservedkeywordsInstance >> ! x3::alnum) ;
        const auto parameter_def = var_decl; // don't know if this is an issue since vardecl is declared later
        const auto parameter_list_def = -(parameter % ',');

        // --- Precedens stuff  
        const auto expression_par_def = ('(' >> expression) > ')';
        const auto expression_base =  expression_par | function_call | var_expression | x3::int_ | x3::bool_;

        const auto expression_def = logical_or;

        // Lowest to highest precedence
        const auto logical_or_def = logical_and >> *logical_or_rhs;
        const auto logical_or_rhs_def = logical_or_op > eq;
        const auto logical_and_def = eq >> *logical_and_rhs;
        const auto logical_and_rhs_def = logical_and_op > eq;
        const auto eq_def = comp >> *eq_rhs;
        const auto eq_rhs_def = eq_op > comp;
        const auto comp_def = term >> *comp_rhs;
        const auto comp_rhs_def = comp_op > term;
        const auto term_def = factor >> *term_rhs;
        const auto term_rhs_def = add_op > factor;
        const auto factor_def = expression_base >> *factor_rhs;
        const auto factor_rhs_def = mul_op > expression_base;
        // Precedens stuff ---

        const auto var_expression_def = id;
        
        const auto var_assign_def = (id >> '=' >> expression) > ";";
        const auto while_statement_def = optionalParExp("while") > block;

      
        const auto print_statement_def = x3::lit("print") > '(' > expression > ')' > ';';
        const auto return_statement_def = optionalParExp("return") > ';';
        const auto block_line_def = statement | decl;
        const auto block_def = '{' > *block_line > '}';


        const auto else_if = x3::lit("else ") >> if_statement;
        const auto conditional_statement_def = if_statement > *(else_if) > -else_statement;
                
        const auto if_statement_def = optionalParExp("if") > block;
        const auto else_statement_def = x3::lit("else") > block;

        const auto func_decl_def = type >> id >> ('(' > parameter_list > ')' > block);
        const auto var_decl_def = type >> id;
        const auto var_decl_assign_def = var_decl >> ( '=' > expression > ';');
        const auto var_decl_statement_def = var_decl >> ';';

        const auto statement_expression_def = expression >> ';';
        const auto statement_def = var_assign | var_decl_statement | var_decl_assign | conditional_statement | while_statement | statement_expression | print_statement | return_statement;
        
        // const auto decl_def = var_decl | func_decl;
        const auto decl_def = var_decl_assign | var_decl_statement | func_decl;
        const auto prog_def = *decl;

        const auto function_call_def = (id >> '('>> argument_list) > ')';
        const auto argument_list_def = -(expression % ',');

        // Not useable, TODO
        const auto array_type_def = type >> x3::lit("[]");

        BOOST_SPIRIT_DEFINE(
            factor,
            factor_rhs,
            term,
            term_rhs,
            comp,
            comp_rhs,
            eq,
            eq_rhs,
            logical_or,
            logical_or_rhs,
            logical_and,
            logical_and_rhs,
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
            argument_list,
            expression_par,
            statement_expression,
            statement,
            print_statement,
            return_statement,
            var_expression,
            var_decl_statement,
            var_decl_assign,
            if_statement,
            else_statement,
            conditional_statement
        )

        grammar::ast::Prog parse(std::string_view src)
        {
            namespace x3 = boost::spirit::x3;

            grammar::ast::Prog ast;
            using PosIter = boost::spirit::line_pos_iterator<std::string_view::const_iterator>;
            PosIter iter(src.begin());

            const auto makeError = [&]() {
                const auto lineNumber = iter.position();
                const auto lineRange = get_current_line(PosIter(src.begin()), iter, PosIter(src.end()));
                const auto column = get_column(lineRange.begin(), iter, SpacesPerTabs);
                std::string msg = "Parsing failed at " + std::to_string(lineNumber) + ":" + std::to_string(column) + ":\n";
                for(const char c : lineRange) {
                    if(c == '\t') msg += std::string(SpacesPerTabs, ' ');
                    else msg += c;
                }
                msg += "\n";
                msg += std::string(column - 1, '-');
                msg += "^";
                return msg;
	        };

            try {
                bool r = phrase_parse(iter,  PosIter(src.end()), prog, x3::space, ast);

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




