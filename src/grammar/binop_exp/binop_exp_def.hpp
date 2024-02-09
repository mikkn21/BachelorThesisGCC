#ifndef BINOP_EXP_DEF_HPP
#define BINOP_EXP_DEF_HPP

#include <boost/spirit/home/x3.hpp>

#include "binop_exp_ast.hpp"
#include "binop_exp_ast_adapt.hpp"
#include "binop_exp.hpp"

#include "../integer/integer_def.hpp"

namespace grammar 
{

    // our integer parser def 
    namespace parser 
    {
        namespace x3 = boost::spirit::x3; 
        namespace ascii = x3::ascii; 

        using x3::int_;
        using x3::char_;

        using namespace grammar::binop_exp_ast;

        x3::rule<class binop_exp, binop_exp_ast::binop_exp> const binop_exp = "binop_exp"; 

        // Define a parser for operators
        auto const operator_parser = 
            x3::string("==") [([](auto& ctx) { _val(ctx).op = grammar::binop_exp_ast::op_eq; })] |

            x3::string("!=") [([](auto& ctx) { _val(ctx).op = grammar::binop_exp_ast::op_neq; })]   |
            x3::string("<=") [([](auto& ctx) { _val(ctx).op = grammar::binop_exp_ast::op_leq; })]   ;
            // x3::string(">=") [([](auto& ctx) { _val(ctx) = op_geq; })]   |
            // x3::char_('+')   [([](auto& ctx) { _val(ctx) = op_add; })]   |
            // x3::char_('-')   [([](auto& ctx) { _val(ctx) = op_minus; })] |
            // x3::char_('*')   [([](auto& ctx) { _val(ctx) = op_times; })] |
            // x3::char_('/')   [([](auto& ctx) { _val(ctx) = op_div; })]   |
            // x3::char_('%')   [([](auto& ctx) { _val(ctx) = op_mod; })]   |
            // x3::char_('<')   [([](auto& ctx) { _val(ctx) = op_lt; })]    |
            // x3::char_('>')   [([](auto& ctx) { _val(ctx) = op_gt; })]    |
            // x3::char_('&')   [([](auto& ctx) { _val(ctx) = op_and; })]   |
            // x3::char_('|')   [([](auto& ctx) { _val(ctx) = op_or; })];

        auto const binop_exp_def = 
            parser::integer
            >> operator_parser 
            >> parser::integer
            ;   

        BOOST_SPIRIT_DEFINE(binop_exp); 

    } // namespace parser 

    parser::binop_exp_type binop_exp() 
    {
        return parser::binop_exp;
    }

} // namespace grammar

#endif