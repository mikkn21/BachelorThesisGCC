#ifndef MGRAMMAR_AST_ADAPT_HPP
#define MGRAMMAR_AST_ADAPT_HPP 

#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::BinopExps,
    (grammar::ast::Expression, lhs),
    (std::vector<grammar::ast::Rhs>, rhss)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::Rhs,
    (std::string, op),
    (grammar::ast::Expression, exp)
)


BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::PrintStatement,
    (grammar::ast::Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::VarExpression,
    (grammar::ast::Id, id)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::IfStatement,
    (grammar::ast::Expression, exp),
    (grammar::ast::Block, block)
)


BOOST_FUSION_ADAPT_STRUCT(
   grammar::ast::ElseStatement,
    (grammar::ast::Block, block)
)


BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::ConditionalStatement,
    (grammar::ast::IfStatement, ifStatement),
    (std::vector<grammar::ast::IfStatement>, elseIfs), 
    (boost::optional<grammar::ast::ElseStatement>, conditionalElse)
    // (x3::variant<ElseStatement, x3::unused_type>, conditionalElse)
)


BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::ReturnStatement,
    (grammar::ast::Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::StatementExpression, 
    (grammar::ast::Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::Id,
    (std::string, id)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::ExpressionPar,
    (grammar::ast::Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::ArgumentList, 
    (std::vector<grammar::ast::Expression>, arguments)
)
 

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::PrimitiveType,
    (std::string, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::FunctionCall, 
    (grammar::ast::Id, id), 
    (grammar::ast::ArgumentList, argument_list)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::Block,
    (std::vector<grammar::ast::BlockLine>, block_line)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::VarDecl,
    (grammar::ast::Type, type)
    (grammar::ast::Id, id)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::VarDeclAssign,
    (grammar::ast::VarDecl, decl)
    (grammar::ast::Expression, exp)
)


BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::ParameterList,
    (std::vector<grammar::ast::Parameter>, parameter)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::FuncDecl,
    (grammar::ast::Type, type)
    (grammar::ast::Id, id)
    (grammar::ast::ParameterList, parameter_list)
    (grammar::ast::Block, block)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::ArrayType,
    (grammar::ast::Type, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::VarAssign,
    (grammar::ast::Id, id),
    (grammar::ast::Expression, exp)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::WhileStatement,
    (grammar::ast::Expression, exp),
    (grammar::ast::Block, block)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ast::Prog,
    (std::vector<grammar::ast::Decl>, decls)
)


#endif