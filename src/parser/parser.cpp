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

#include <iostream>



namespace grammar {
    namespace parser {

        namespace x3 = boost::spirit::x3;
    
        namespace {
            constexpr int space_per_tabs = 4;   
        } // namespace

        struct LocationHandler {
            template<typename Iterator, typename Context>
            void on_success(const Iterator &first, const Iterator &last, ast::LocationInfo &loc_info, const Context &) {
                loc_info.line = get_line(first);
            }
        };

        #define RULE(ast, name) \
            struct name##_rule : LocationHandler { }; \
            const x3::rule<struct name##_rule, ast> name = #name ;

        RULE(ast::BinopExps, factor)
        RULE(ast::BinopExps, term)
        RULE(ast::BinopExps, comp)
        RULE(ast::BinopExps, eq)
        RULE(ast::BinopExps, logical_and)
        RULE(ast::BinopExps, logical_or)

        RULE(ast::Rhs, factor_rhs)
        RULE(ast::Rhs, term_rhs)
        RULE(ast::Rhs, comp_rhs)
        RULE(ast::Rhs, eq_rhs)
        RULE(ast::Rhs, logical_and_rhs)
        RULE(ast::Rhs, logical_or_rhs)

        RULE(ast::ClassDecl, class_decl)
        RULE(ast::ObjInst, obj_inst)
        RULE(ast::ClassType, class_type)
        RULE(ast::IdAccess, id_access)

        RULE(ast::BreakStatement, break_statement)
        RULE(ast::ContinueStatement, continue_statement)
        RULE(ast::BetaExpression, beta_expression)

        RULE(ast::Id, id)
        RULE(ast::PrimitiveType, primitive_type)
        RULE(ast::BlockLine, block_line)
        RULE(ast::Block, block)

        RULE(ast::Type, type)
        RULE(ast::VarDecl, var_decl)
        RULE(ast::VarDeclAssign, var_decl_assign)
        RULE(ast::VarDeclStatement, var_decl_statement)

        RULE(ast::Parameter, parameter)
        RULE(ast::ParameterList, parameter_list)

        RULE(ast::FuncDecl, func_decl)
        RULE(ast::VarAssign, var_assign)
        RULE(ast::WhileStatement, while_statement)

        RULE(ast::Decl, decl)
        RULE(ast::Prog, prog)
        RULE(ast::Expression, expression)
        RULE(ast::ExpressionPar, expression_par)

        RULE(ast::FunctionCall, function_call)
        RULE(ast::ArgumentList, argument_list)
        RULE(ast::StatementExpression, statement_expression)
        RULE(ast::Statement, statement)

        RULE(ast::PrintStatement, print_statement)
        RULE(ast::ReturnStatement, return_statement)
        RULE(ast::VarExpression, var_expression)

        RULE(ast::ArrayType, array_type)
        RULE(ast::ArrayIndex, array_index)
        RULE(ast::ArrayIndexExp, array_index_exp)
        RULE(ast::ArrayInitExp, array_init_exp)
        RULE(ast::ArrayIndexAssign, array_index_assign)

        RULE(ast::IfStatement, if_statement)
        RULE(ast::ElseStatement, else_statement)
        RULE(ast::ConditionalStatement, conditional_statement)
        
        #undef RULE

       
        struct reserved_keywords : x3::symbols<std::string> {
            reserved_keywords() {
                add("if", "if")
                    ("else", "else")
                    ("while", "while")
                    ("break", "break")
                    ("continue", "continue")
                    ("return", "return")
                    ("int", "int")
                    ("bool", "bool")
                    ("true", "true")
                    ("false", "false")
                    ("new", "new")
                    ("beta", "beta")
                    ("print", "print"); 
            }
        } reserved_keywords_instance; 


        // highest to lowset precedence
        const auto mul_op = x3::string("*") | x3::string("/") | x3::string("%");
        const auto add_op = x3::string("+") | x3::string("-");
        const auto comp_op = x3::string("<=") | x3::string(">=") | x3::string("<") | x3::string(">");
        const auto eq_op = x3::string("==") | x3::string("!=");
        const auto logical_and_op = x3::string("&");
        const auto logical_or_op = x3::string("|");

        auto optional_par_exp(std::string s) {
            return (x3::lit(s + " ") > expression) | (x3::lit(s) > expression_par);
        }


        // Useable
        const auto primitive_type_def = x3::string("int") | x3::string("bool");
        const auto type_def = array_type | primitive_type | class_type;
        const auto id_def = x3::raw[ x3::lexeme[( x3::char_("a-zA-Z_") >> * boost::spirit::x3::char_("a-zA-Z_0-9"))]] - (reserved_keywords_instance >> ! x3::alnum) ;
        const auto parameter_def = var_decl; 
        const auto parameter_list_def = -(parameter % ',');
 
        // --- Precedens stuff  
        const auto expression_par_def = ('(' >> expression) > ')';
        const auto expression_base =  expression_par | beta_expression | array_init_exp | obj_inst | function_call | array_index_exp | var_expression | id_access | x3::int_ | x3::bool_;

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

        const auto var_expression_def = id_access;
        
        const auto var_assign_def = (id_access >> '=' >> expression) > ";";
        const auto while_statement_def = optional_par_exp("while") > block;

        const auto print_statement_def = x3::lit("print") > '(' > expression > ')' > ';';
        const auto return_statement_def = optional_par_exp("return") > ';';
        const auto block_line_def = statement | decl;
        const auto block_def = '{' > *block_line > '}';

        const auto else_if = x3::lit("else ") >> if_statement;
        const auto conditional_statement_def = if_statement > *(else_if) > -else_statement;
                
        const auto if_statement_def = optional_par_exp("if") > block;
        const auto else_statement_def = x3::lit("else") > block;

        const auto func_decl_def = type >> id >> ('(' > parameter_list > ')' > block);
        const auto var_decl_def = type >> id;
        const auto var_decl_assign_def = var_decl >> ( '=' > expression > ';');
        const auto var_decl_statement_def = var_decl >> ';';

        const auto statement_expression_def = expression >> ';';
        const auto statement_def = conditional_statement | while_statement | print_statement | return_statement | break_statement | continue_statement | var_assign | array_index_assign | statement_expression;

        const auto break_statement_def = x3::lit("break") > ';';
        const auto continue_statement_def = x3::lit("continue") > ';';
        const auto beta_expression_def = x3::string("beta");

        const auto class_decl_def = x3::lit("class ") > id > '{' > *var_decl_statement > '}';
        const auto obj_inst_def = x3::lit("new ") > id > '(' > argument_list > ')';
        const auto class_type_def = id;
        const auto id_access_def = id % '.' ;

        const auto decl_def = var_decl_assign | var_decl_statement | func_decl | class_decl;
        const auto prog_def = *decl;

        const auto function_call_def = (id >> '('>> argument_list) > ')';
        const auto argument_list_def = -(expression % ',');

        const auto array_type_def = (primitive_type >> '[') > x3::int_ > ']'; // NOTE: Is literal int this is by choice
        const auto array_init_exp_def = (x3::lit("new ") >> primitive_type >> '[') > (expression % ',') > ']';
        const auto array_index_exp_def = array_index;
        const auto array_index_def = (id_access >> '[') > (expression % ',') > ']';
        const auto array_index_assign_def = (array_index >> '=') > expression > ';';

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
            array_init_exp,
            array_index,
            array_index_exp,
            array_index_assign,
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
            conditional_statement,
            class_decl,
            obj_inst,
            class_type,
            id_access,
            break_statement,
            continue_statement,
            beta_expression
        )

        grammar::ast::Prog parse(std::string_view src)
        {
            namespace x3 = boost::spirit::x3;

            grammar::ast::Prog ast;
            using PosIter = boost::spirit::line_pos_iterator<std::string_view::const_iterator>;
            PosIter iter(src.begin());

            const auto make_error = [&]() {
                const auto line_number = iter.position();
                const auto line_range = get_current_line(PosIter(src.begin()), iter, PosIter(src.end()));
                const auto column = get_column(line_range.begin(), iter, space_per_tabs);
                std::string msg = "Parsing failed at " + std::to_string(line_number) + ":" + std::to_string(column) + ":\n";
                for(const char c : line_range) {
                    if(c == '\t') msg += std::string(space_per_tabs, ' ');
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
                    throw SyntaxError(make_error() + "\nEnd of x3 error.");
                }
                return ast;

            } catch (const x3::expectation_failure<PosIter> &e) {
	            iter = e.where();
		        throw SyntaxError(make_error()
		            + "\nExpected " + e.which() + ".\n"
		            + "End of x3 error.");
            }
        }
    } // namespace parser

} // namespace grammar




