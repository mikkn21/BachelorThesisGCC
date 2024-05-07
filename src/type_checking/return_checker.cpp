#include "return_checker.hpp"
#include "../visitor.hpp"
#include "iostream"

class ReturnCheckVisitor : boost::static_visitor<bool> {
    
public:
    
    template <typename T>
    bool operator()(boost::spirit::x3::forward_ast<T> &ast) {
        return (*this)(ast.get());
    }

    // For everything that is not a Block or ConditionalStatement, we do not guarantee that it will always return
    template <typename T>
    bool operator()(T &node) {
        return false;
    }

    bool operator()(grammar::ast::Statement &statement) {
        return apply_visitor(*this, statement);
    }

    bool operator()(grammar::ast::BlockLine &block_line) {
        return apply_visitor(*this, block_line);
    }
    
    bool operator()(grammar::ast::Block &block) {
        for (auto &block_line : block.block_line) {
            if ((*this)(block_line)) {
                return true;
            }
        }
        return false;
    }

    bool operator()(grammar::ast::ReturnStatement &return_statement) {
        return true;
    }
    
    bool operator()(grammar::ast::ConditionalStatement &conditional_statement) {
        // if no else block exists; we cannot guarantee that it will always return
        if (conditional_statement.conditional_else.get_ptr() == nullptr) {
            return false;
        } 

        if (!(*this)(conditional_statement.if_statement.block)) {
            return false;
        }

        for (auto &else_if : conditional_statement.else_if) {
            if (!(*this)(else_if.block)) {
                return  false;
            }
        } 
        
        if (!(*this)(conditional_statement.conditional_else->block)) {
            return  false;
        }

        return true;
    }
};

class FuncFinderVisitor : public Visitor {

    ReturnCheckVisitor return_checker;

    void pre_visit(grammar::ast::FuncDecl &func_decl) override {
        if (!return_checker(func_decl.block)) {
            throw TypeCheckError("Function " + func_decl.id.id + " does not always return a value", func_decl);
        }
    }

};

void return_checker(grammar::ast::Prog &prog) {
    FuncFinderVisitor return_checker;
    auto traveler = TreeTraveler(return_checker);
    traveler(prog);
}
