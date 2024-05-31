#include "../visitor.hpp"
#include "semantics_error.hpp"
#include "symbol_table.hpp"
#include <iostream>
#include "symbol_collection_phase2.hpp"
#include <memory>


class SymbolCollectionVisitorPhase2 : public Visitor {
private:
    SymbolTable *current_symbol_table; // Has to be a pointer, not a reference!!!

public: 
    
    SymbolCollectionVisitorPhase2(grammar::ast::Prog &prog) : Visitor(), current_symbol_table(prog.global_scope) { }
   
    void pre_visit(grammar::ast::FuncDecl &func_decl) override {
        current_symbol_table = func_decl.sym->sym_tab;
    }

    void post_visit(grammar::ast::FuncDecl &func_decl) override {
        current_symbol_table = current_symbol_table->parent_scope;
        func_decl.sym->type.return_type = std::make_shared<SymbolType>(convert_type(func_decl.type));
        std::vector<SymbolType> parameters = std::vector<SymbolType>();
        for (auto &parameter : func_decl.parameter_list.parameters) {
            parameters.push_back(convert_type(boost::get<grammar::ast::VarDecl>(parameter.var).type));
        }
        func_decl.sym->type.parameters = parameters;
    }

    void pre_visit(grammar::ast::ClassDecl &class_decl) override {
        ClassSymbol *classSymbol = dynamic_cast<ClassSymbol *>(class_decl.id.sym);
        // ClassDecl shouldn't have a symbol that is not a ClassSymbol
        current_symbol_table = classSymbol->symbol_table;
    }

    void post_visit(grammar::ast::ClassDecl &class_decl) override {
        current_symbol_table = current_symbol_table->parent_scope;
    }

    void post_visit(grammar::ast::FunctionCall &func_call) override { 
        Symbol *sym = current_symbol_table->find(func_call.id.id);
        if (sym == nullptr) {
            throw SemanticsError(func_call.id.id + " not declared in scope5", func_call);
        }

        if (auto fund_sym = dynamic_cast<FuncSymbol *>(sym)) {
            func_call.id.sym = fund_sym;
        } else {
            throw SemanticsError("A non-function was used as a function", func_call);
        }
    }

    void post_visit(grammar::ast::ClassType &class_type) override {
        Symbol *sym = current_symbol_table->find(class_type.id.id);
        if (sym == nullptr) {
            throw SemanticsError(class_type.id.id + " not declared in scope6", class_type);
        }

        if (auto class_sym = dynamic_cast<ClassSymbol *>(sym)) {
            class_type.id.sym = class_sym;
        } else {
            throw SemanticsError("A non-class was used as a class", class_type);
        }
    }
    
    void post_visit(grammar::ast::VarDecl &var_decl) override {
        SymbolType symbol_type = convert_type(var_decl.type);
        var_decl.sym->type = symbol_type;
    }

    void pre_visit(grammar::ast::ObjInst &obj_inst) override {
        Symbol *sym = current_symbol_table->find(obj_inst.id.id);
        if (sym == nullptr) {
            throw SemanticsError(obj_inst.id.id + " not declared in scope7", obj_inst);
        }
        obj_inst.id.sym = sym;
    }

    void post_visit(grammar::ast::IdAccess &id_access) override {

        SymbolTable *current_scope;
        // Get the scope of the first class variable
        if (auto var_symbol = dynamic_cast<VarSymbol *>(id_access.ids[0].sym)) {
            if (auto *class_symbol_type = boost::get<ClassSymbolType>(&var_symbol->type)) {
                current_scope = class_symbol_type->symbol->symbol_table;
            } else if (id_access.ids.size() > 1) {
                throw SemanticsError("Attempted to access a non-class", id_access.ids[0]);
            }
        }

        // Make the sure the next IDs exist within their corresponding scopes
        // and link them up with their symbols
        for (size_t i = 1; i < id_access.ids.size(); i++) {
            Symbol *symbol = current_scope->find(id_access.ids[i].id);
            if (symbol == nullptr) {
                throw SemanticsError(id_access.ids[i].id + " not declared in scope8", id_access);
            } 
            id_access.ids[i].sym = symbol;

            if ( auto var_symbol = dynamic_cast<VarSymbol *>(symbol)) {
                if (auto *class_symbol_type = boost::get<ClassSymbolType>(&var_symbol->type)) {
                    current_scope = class_symbol_type->symbol->symbol_table;
                } else if (i != id_access.ids.size() - 1) {
                    throw SemanticsError("Attempted to access a non-class", id_access.ids[i]);
                }
            }
        }  
    }
};

void symbol_collection_phase2(grammar::ast::Prog &prog) {
    auto visitor = SymbolCollectionVisitorPhase2(prog);
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
}

