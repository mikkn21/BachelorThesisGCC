#include "code_generation.hpp"
#include "../semantics/symbol_table.hpp"
#include "../visitor.hpp"
#include "ir.hpp"
#include "link_instructions.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <stack>

const int callee_offset = -40; // TODO: this should be referenced from a shared file so all occurences of this has a common variable. Or it should be removed

using AstValue = std::variant<int, bool, GenericRegister>;

class FunctionManager {
private:
    std::stack<Function*> current_function_stack;
    IR *ir;
public:
    FunctionManager() : current_function_stack(), ir(new IR()) {}

    /// adds a new list of instructions to the current scope
    void new_scope(SymbolTable *scope) {
        auto *func = new Function(scope->register_counter, {});
        ir->functions.push_back(func);
        current_function_stack.push(func);
    }

    void new_empty_scope() {
        auto *func = new Function(0, {});
        ir->functions.push_back(func);
        current_function_stack.push(func);
    }
    
    /// pops the current scope
    void end_scope() {
        current_function_stack.pop();
    }

    /// pushes an instruction to the current scope
    void push(Instruction instruction) {
        current_function_stack.top()->code.push_back(instruction);
    }

    GenericRegister new_register() {
        return current_function_stack.top()->new_register();
    }

    GenericRegister get_local_var_register(VarSymbol &var_symbol) {
        return current_function_stack.top()->get_local_var_register(&var_symbol);
    }

    /// get the intermediate code representations.
    IR *get_instructions() {
        return ir;
    }

    long get_stack_counter() {
        return current_function_stack.top()->get_stack_counter();
    }

    long new_stack_slot() {
        return current_function_stack.top()->new_stack_slot();
    }
};


TargetType get_target(AstValue value) {
    if (std::holds_alternative<int>(value)) {
        return ImmediateValue(std::get<int>(value));
    } else if (std::holds_alternative<bool>(value)) {
        return ImmediateValue(std::get<bool>(value));
    } else if (std::holds_alternative<GenericRegister>(value)) {
        return std::get<GenericRegister>(value);
    } else {
        throw IRError("Unexpected type in get_target");
    }
}

class IRVisitor : public Visitor {
public:
    FunctionManager code;

    SymbolTable* global_scope;
    std::stack<grammar::ast::WhileStatement*> while_stack;

    IRVisitor(grammar::ast::Prog &prog) : Visitor(), global_scope(prog.global_scope) {}

private:
    std::vector<std::string> function_container;
    std::stack<AstValue> intermediary_storage;

    GenericRegister static_link_read(SymbolTable &current_scope, VarSymbol &target_var_symbol) {
        if (target_var_symbol.ir_data.is_local) {
            return GenericRegister(target_var_symbol.ir_data.local_id);
        } else {
            size_t depth = current_scope.depth - target_var_symbol.var_decl->id.scope->depth;
            GenericRegister static_link_reg = code.new_register();
            code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(static_link_reg, DIR()), "starting static linking read"));
            for (auto i = 0; i < depth; i++) {
                code.push(Instruction(Op::MOVQ, Arg(static_link_reg, IRL(16)), Arg(static_link_reg, DIR())));
            }
            long target_stack_offset = callee_offset + (target_var_symbol.ir_data.stack_offset + 1) * -8;
            code.push(Instruction(Op::MOVQ, Arg(static_link_reg, IRL(target_stack_offset)), Arg(static_link_reg, DIR()), "fetch value from outer scope"));
            return static_link_reg;
        }
    }

    void static_link_write(SymbolTable &current_scope, VarSymbol &target_var_symbol, TargetType write_value) {
        if (target_var_symbol.ir_data.is_local) {
            code.push(Instruction(Op::MOVQ, Arg(write_value, DIR()), Arg(GenericRegister(target_var_symbol.ir_data.local_id), DIR()), "assign value to local variable"));
        } else {
            GenericRegister static_link_reg = code.new_register();
            size_t depth = current_scope.depth - target_var_symbol.var_decl->id.scope->depth;
            code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(static_link_reg, DIR()), "starting static linking write"));
            for (auto i = 0; i < depth; i++) {
                code.push(Instruction(Op::MOVQ, Arg(static_link_reg, IRL(16)), Arg(static_link_reg, DIR())));
            }
            long target_stack_offset = callee_offset + (target_var_symbol.ir_data.stack_offset + 1) * -8;
            code.push(Instruction(Op::MOVQ, Arg(write_value, DIR()), Arg(static_link_reg, IRL(target_stack_offset)), "assign value to variable in outer scope"));
        }
    }

    void push_caller_save() {
        code.push(Instruction(Op::PUSHQ, Arg(Register::RAX, DIR()), "save rax"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RCX, DIR()), "save rcx"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RDX, DIR()), "save rdx"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RSI, DIR()), "save rsi"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RDI, DIR()), "save rdi"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::R8, DIR()), "save r8"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::R9, DIR()), "save r9"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::R10, DIR()), "save r10"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::R11, DIR()), "save r11"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RSP, DIR()), "save rsp"));
    }

    void push_caller_restore() {
        code.push(Instruction(Op::POPQ, Arg(Register::RSP, DIR()), "restore rsp"));
        code.push(Instruction(Op::POPQ, Arg(Register::R11, DIR()), "restore r11"));
        code.push(Instruction(Op::POPQ, Arg(Register::R10, DIR()), "restore r10"));
        code.push(Instruction(Op::POPQ, Arg(Register::R9, DIR()), "restore r9"));
        code.push(Instruction(Op::POPQ, Arg(Register::R8, DIR()), "restore r8"));
        code.push(Instruction(Op::POPQ, Arg(Register::RDI, DIR()), "restore rdi"));
        code.push(Instruction(Op::POPQ, Arg(Register::RSI, DIR()), "restore rsi"));
        code.push(Instruction(Op::POPQ, Arg(Register::RDX, DIR()), "restore rdx"));
        code.push(Instruction(Op::POPQ, Arg(Register::RCX, DIR()), "restore rcx"));
        code.push(Instruction(Op::POPQ, Arg(Register::RAX, DIR()), "restore rax"));
    }

    void push_callee_save() {
        code.push(Instruction(Op::PUSHQ, Arg(Register::RBX, DIR()), "save RBX"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::R12, DIR()), "save R12"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::R13, DIR()), "save R13"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::R14, DIR()), "save R14"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::R15, DIR()), "save R15"));
        code.push(Instruction(Op::NOTHING , "END OF CALLEE SAVE"));

    }

    void push_callee_restore() {
        code.push(Instruction(Op::POPQ, Arg(Register::R15, DIR()), "restore R15"));
        code.push(Instruction(Op::POPQ, Arg(Register::R14, DIR()), "restore R14"));
        code.push(Instruction(Op::POPQ, Arg(Register::R13, DIR()), "restore R13"));
        code.push(Instruction(Op::POPQ, Arg(Register::R12, DIR()), "restore R12"));
        code.push(Instruction(Op::POPQ, Arg(Register::RBX, DIR()), "restore RBX"));
        code.push(Instruction(Op::NOTHING , "END OF CALLEE restore"));
        
    }


public: 

    void post_visit(grammar::ast::ReturnStatement &return_statement) override {
        auto target = get_target(pop(intermediary_storage));
        code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::RAX, DIR())));

        // Remove local variables from the stack
        code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::RSP, DIR())));
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(callee_offset), DIR()), Arg(Register::RSP, DIR())));
        // The above instruction is actually equal to a subtraction, as 'callee_offset' is a negative value.
        // The intention of these two instructions is to move the stack pointer up to right after the calle saves.

        push_callee_restore();
        code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::RSP, DIR())));
        code.push(Instruction(Op::POPQ, Arg(Register::RBP, DIR())));
        code.push(Instruction(Op::RET));
    }

    void pre_visit(grammar::ast::FuncDecl &func_decl) override {
        code.new_scope(func_decl.sym->sym_tab);
        code.push(Instruction(Op::LABEL, Arg(Label(func_decl.label), DIR())));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RBP, DIR()), "save old rbp"));
        code.push(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RBP, DIR()), "set rbp for function scope"));
        // code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_SAVE, DIR())));
        push_callee_save();
        

        std::vector<VarSymbol*> var_decls = func_decl.sym->sym_tab->get_var_symbols();

        for (size_t i = 0; i < var_decls.size(); i++) {
            auto &data = var_decls[i]->ir_data;
            if (!data.is_local) {
                data.stack_offset = code.new_stack_slot();
                code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()), "initialize variable referenced from other scopes to 0"));
            }
        }
    }

    void post_visit(grammar::ast::FuncDecl &func_decl) override {
        code.end_scope();
    }

    void post_visit(grammar::ast::FunctionCall &func_call) override {
        GenericRegister result = code.new_register(); // register for the function result to be stored in

        // code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_SAVE, DIR())));
        push_caller_save();
        // add arguments to stack in reverse order
        for (size_t i = 0; i < func_call.argument_list.arguments.size(); i++) {
            //AstValue value = pop(intermediary_storage);
            auto target = get_target(pop(intermediary_storage));
            code.push(Instruction(Op::PUSHQ, Arg(target, DIR()), "pushing register argument"));
        }
        int callee_depth = dynamic_cast<FuncSymbol*>(func_call.id.sym)->sym_tab->parent_scope->depth;
        int caller_depth = func_call.id.scope->depth;
        int difference = caller_depth - callee_depth;
        
        GenericRegister static_link_reg = code.new_register();
        code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(static_link_reg, DIR()), "calculating static link for function call"));
        for (auto i = 0; i < difference; i++) {
            code.push(Instruction(Op::MOVQ, Arg(static_link_reg, IRL(16)), Arg(static_link_reg, DIR())));
        } 

        code.push(Instruction(Op::PUSHQ, Arg(static_link_reg, DIR()), "setting static link")); // Settting static link.
        std::string label = dynamic_cast<FuncSymbol*>(func_call.id.sym)->func_decl->label;
        code.push(Instruction(Op::CALL, Arg(Label(label), DIR())));
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue((func_call.argument_list.arguments.size()+1) * 8), DIR()), Arg(Register::RSP, DIR()), "remove arguments and static link from stack")); 
        code.push(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR()), "save result from function call")); 

        push_caller_restore();
        intermediary_storage.push(result); // pushing the result of the function
    }

    VarSymbol *get_var_symbol(Symbol *symbol) {
        if (auto var_symbol = dynamic_cast<VarSymbol *>(symbol)) {
            return var_symbol;
        } else {
            throw IRError("Attempted to use a symbol as a VarSymbol, despite not being one");
        }
    }


    void post_visit(grammar::ast::VarAssign &var_assign) override {
        auto target = get_target(pop(intermediary_storage));
        std::vector<VarSymbol*> var_symbols;
        for (auto id : var_assign.id_access.ids) {
            var_symbols.push_back(get_var_symbol(id.sym));
        }

        if (var_assign.id_access.ids.size() > 1) {
            SymbolTable &current_scope = *var_assign.id_access.ids.front().scope;
            VarSymbol &target_var_symbol = *var_symbols.front();
            GenericRegister result = static_link_read(current_scope, target_var_symbol);
            GenericRegister reg1 = code.new_register();
            
            code.push(Instruction(Op::MOVQ, Arg(result, DIR()), Arg(reg1, DIR()), "Initialise static link for object access"));
            for (size_t i = 1; i < var_symbols.size()-1; i++) {
            //                                          -1 because heap memory is 0 inxed, but stack memory is not            
                code.push(Instruction(Op::MOVQ, Arg(reg1, IRL((var_symbols[i]->ir_data.local_id-1) * 8)), Arg(reg1, DIR()), "accessing member relative to it's scope")); /// for the first access this is relative to current scope
            } // by the end of this loop the scope / block of data where varAssign.idAccess.back() is located should be in R8

            //                                          -1 because heap memory is 0 inxed, but stack memory is not            
            code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(reg1, IRL((var_symbols.back()->ir_data.local_id-1) * 8)), "inserting value into found member"));
        } else {
            VarSymbol *var_symbol = get_var_symbol(var_assign.id_access.ids.back().sym);
            auto current_scope = var_assign.id_access.ids.back().scope;
            static_link_write(*current_scope, *var_symbol, target);
        }        
    }

    void post_visit(grammar::ast::VarDeclAssign &var_decl_assign) override {
        auto target = get_target(pop(intermediary_storage));
        SymbolTable *current_scope = var_decl_assign.decl.id.scope;
        auto target_var_symbol = var_decl_assign.decl.sym;
        static_link_write(*current_scope, *target_var_symbol, target);
    }


    void pre_visit(int &i) override {
        intermediary_storage.push(i);
    }

    void pre_visit(bool &b) override {
        intermediary_storage.push(b);
    }

    void pre_visit(grammar::ast::BetaExpression &beta) override {
        intermediary_storage.push(0);
    }

    void post_visit(grammar::ast::VarExpression &var_expr) override {
        if (var_expr.id_access.ids.size() > 1){
            auto frontId = var_expr.id_access.ids.front();
            auto &read_var_symbol = *get_var_symbol(frontId.sym); // auto frontLocalId = frontSym->local_id;
            auto &current_scope = *var_expr.id_access.ids.back().scope;
            GenericRegister read_register = static_link_read(current_scope, read_var_symbol);
            
            GenericRegister reg1 = code.new_register();

            code.push(Instruction(Op::MOVQ, Arg(read_register, DIR()), Arg(reg1, DIR()), "Store static link for object access"));
            for (size_t i = 1; i < var_expr.id_access.ids.size()-1; i++) {
            //                                          -1 because heap memory is 0 inxed, but stack memory is not            
                code.push(Instruction(Op::MOVQ, Arg(reg1, IRL((get_var_symbol(var_expr.id_access.ids[i].sym)->ir_data.local_id - 1) * 8)), Arg(reg1, DIR()), "accessing member relative to it's scope")); // for the first access this is relative to current scope
            }
            //                                          -1 because heap memory is 0 inxed, but stack memory is not            
            code.push(Instruction(Op::MOVQ, Arg(reg1, IRL((get_var_symbol(var_expr.id_access.ids.back().sym)->ir_data.local_id-1) * 8)), Arg(reg1, DIR()), "get value from member of class and save to temporary register")); 
            intermediary_storage.push(reg1);
        } else {
            auto frontId = var_expr.id_access.ids.front();
            auto &target_var_symbol = *get_var_symbol(frontId.sym); // auto frontLocalId = frontSym->local_id;
            auto &current_scope = *var_expr.id_access.ids.back().scope;
            GenericRegister result_register = static_link_read(current_scope, target_var_symbol);
            intermediary_storage.push(result_register);
        }
    }

    void post_visit(grammar::ast::Rhs &op_exp) override {
        // future optimization: calculate immediate values immediately to optimize program.
        GenericRegister result = code.new_register(); 
        auto r_target = get_target(pop(intermediary_storage));
        auto l_target = get_target(pop(intermediary_storage));

        GenericRegister lefthand = code.new_register();
        GenericRegister righthand = code.new_register();

        code.push(Instruction(Op::MOVQ, Arg(l_target, DIR()), Arg(lefthand, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(r_target, DIR()), Arg(righthand, DIR())));

        auto &op = op_exp.op;
        if (op == "+") {
            code.push(Instruction(Op::ADDQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(lefthand, DIR()), Arg(result, DIR())));
        } else if (op == "-") {
            code.push(Instruction(Op::SUBQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(lefthand, DIR()), Arg(result, DIR())));
        } else if (op == "*") {
            code.push(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
            code.push(Instruction(Op::IMULQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(lefthand, DIR()), Arg(result, DIR())));
        } else if (op == "/") {
            code.push(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(lefthand, DIR()), Arg(Register::RAX, DIR())));
            code.push(Instruction(Op::IDIVQ, Arg(righthand, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR())));
        } else if (op == "%") {
            code.push(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(lefthand, DIR()), Arg(Register::RAX, DIR())));
            code.push(Instruction(Op::IDIVQ, Arg(righthand, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::RDX, DIR()), Arg(result, DIR())));
        } else if (op == "&") {
            code.push(Instruction(Op::ANDQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(lefthand, DIR()), Arg(result, DIR())));
        } else if (op == "|") {
            code.push(Instruction(Op::ORQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(lefthand, DIR()), Arg(result, DIR())));
        } else if (op == "<") {
            code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
            code.push(Instruction(Op::CMPQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::SETL, Arg(Register::R10B, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        } else if (op == ">") {
            code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
            code.push(Instruction(Op::CMPQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::SETG, Arg(Register::R10B, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        } else if (op == "==") {
            code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR()), "start of equal compare"));
            code.push(Instruction(Op::CMPQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::SETE, Arg(Register::R10B, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR()), "end of equal compare"));
        } else if (op == "!=") {
            code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
            code.push(Instruction(Op::CMPQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::SETNE, Arg(Register::R10B, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        } else if (op == "<=") {
            code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR()), "start of less than or equal compare"));
            code.push(Instruction(Op::CMPQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::SETLE, Arg(Register::R10B, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        } else if (op == ">=") {
            code.push(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
            code.push(Instruction(Op::CMPQ, Arg(righthand, DIR()), Arg(lefthand, DIR())));
            code.push(Instruction(Op::SETGE, Arg(Register::R10B, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
        }

        intermediary_storage.push(result);  
    }

    void post_visit(grammar::ast::ArrayInitExp &arr) override {
        std::vector<AstValue> sizes;
        for (auto size : arr.sizes) {
            sizes.push_back(pop(intermediary_storage));
        }

        TargetType mem_size = code.new_register(); 


        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(8), DIR()), Arg(mem_size, DIR()), "initialize memory size" ));
        for (auto value : sizes) {
            code.push(Instruction(Op::IMULQ, Arg(get_target(value), DIR()), Arg(mem_size, DIR()), "calculate memory"));
        }
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(arr.sizes.size() * 8), DIR()), Arg(mem_size, DIR()), "found size of memory"));

        // allocate memory
        code.push(Instruction(Op::MOVQ, Arg(mem_size, DIR()), Arg(Register::RDI, DIR()), "set memory size for allocation" ));
        code.push(Instruction(Op::CALL, Arg(Label("allocate"), DIR()), "allocate memory of found memory size"));
        
        GenericRegister arrayStart = code.new_register();
        code.push(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(arrayStart, DIR()), "Save array pointer in generic register" ));

        // Set the size of each dimension of  the array
        for (size_t i = 0; i < sizes.size(); i++) {
            code.push(Instruction(Op::MOVQ, Arg(get_target(sizes[i]), DIR()), Arg(arrayStart, IRL(long(i * 8))), "set size of dimension " + std::to_string(i + 1)));
        }

        // setup zero initialization loop
        GenericRegister counter = code.new_register();

        code.push(Instruction(Op::MOVQ, Arg(arrayStart, DIR()), Arg(counter, DIR()), "initialize counter for initialization loop"));
        code.push(Instruction(Op::ADDQ, Arg(mem_size, DIR()), Arg(arrayStart, DIR()), "Store end of array"));
        // NOTE: arrayStart is now the end of the array
        code.push(Instruction(Op::LABEL, Arg(Label(arr.loop_label), DIR())));

        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(0), DIR()), Arg(counter, IND()), "Set array element value to 0"));
        
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(8), DIR()), Arg(counter, DIR()), "increment counter"));
       
        code.push(Instruction(Op::CMPQ, Arg(arrayStart, DIR()), Arg(counter, DIR()), "check if we are done initializing"));
        code.push(Instruction(Op::JG, Arg(Label(arr.loop_label), DIR()), "if we are done initializing, jump to end of initialization loop"));

        code.push(Instruction(Op::SUBQ, Arg(mem_size, DIR()), Arg(arrayStart, DIR()), "restore end of array to be start of array"));
        // NOTE: arrayStart is now the start of the array again
        // set the array pointer to point to the first element of the array
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(sizes.size() * 8), DIR()), Arg(arrayStart, DIR()), "set array pointer to point to first element" ));
        
        intermediary_storage.push(arrayStart);
    }


    void post_visit(grammar::ast::ArrayIndex &index) override {
        std::vector<TargetType> index_targets;
        for (size_t i = 0; i < index.indices.size(); i++) {
            index_targets.push_back(get_target(pop(intermediary_storage)));
        }

        // Static link
        SymbolTable &current_scope = *index.id_access.ids.front().scope;
        VarSymbol *target_symbol = get_var_symbol(index.id_access.ids.back().sym);
        GenericRegister array_ptr = static_link_read(current_scope, *target_symbol);

        // Check null
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(0), DIR()), Arg(array_ptr, DIR()), "Start checking for beta"));
        code.push(Instruction(Op::JNE, Arg(Label(index.beta_check_label), DIR())));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(index.line), DIR()), Arg(Register::RDI, DIR()), "Line number"));
        //code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_SAVE, DIR())));
        push_caller_save();
        code.push(Instruction(Op::CALL, Arg(Label("print_is_beta"), DIR())));
        //code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_RESTORE, DIR())));
        push_caller_restore();
        code.push(Instruction(Op::LABEL, Arg(Label(index.beta_check_label), DIR())));

        // TODO: Check valid index

        // Get a pointer to the correct index
        GenericRegister index_address = code.new_register();
        GenericRegister intermediate_product = code.new_register();
        GenericRegister intermediate_value = code.new_register();
        // NOTE: indexTargets are in reverse order such that the last index is first
        code.push(Instruction(Op::MOVQ, Arg(array_ptr, DIR()), Arg(index_address, DIR()), "Initialize the index address"));
        code.push(Instruction(Op::MOVQ, Arg(index_targets.front(), DIR()), Arg(intermediate_value, DIR()), "Initialize the intermediate value"));
        code.push(Instruction(Op::IMULQ, Arg(ImmediateValue(8), DIR()), Arg(intermediate_value, DIR()), "Translate the first index to byte address"));
        code.push(Instruction(Op::ADDQ, Arg(intermediate_value, DIR()), Arg(index_address, DIR()), "Add the first index to the address"));

        // Multi-dimensional array indexing
        if (index.indices.size() > 1) {
            code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(intermediate_product, DIR()), "Initialize the product"));
            for (size_t i = 1; i < index.indices.size(); i++) {
                code.push(Instruction(Op::IMULQ, Arg(array_ptr, IRL(long(-i * 8))), Arg(intermediate_product, DIR()), "Multiply with the previous product"));
                code.push(Instruction(Op::MOVQ, Arg(index_targets[i], DIR()), Arg(intermediate_value, DIR()), "Initialize the intermediate value"));
                code.push(Instruction(Op::IMULQ, Arg(intermediate_product, DIR()), Arg(intermediate_value, DIR()), "Multiply the intermediate value and product"));
                code.push(Instruction(Op::IMULQ, Arg(ImmediateValue(8), DIR()), Arg(intermediate_value, DIR()), "Translate the intermediate value to a byte address"));
                code.push(Instruction(Op::ADDQ, Arg(intermediate_value, DIR()), Arg(index_address, DIR()), "Add the intermediate value to the address"));
            }
        }

        intermediary_storage.push(index_address);
    }



    void post_visit(grammar::ast::ArrayIndexExp &index_exp) override {
        TargetType index_ptr = get_target(pop(intermediary_storage));
        auto index_ptr_generic = std::get<GenericRegister>(index_ptr); // index_ptr is the production of ArrayIndex and thus will always be a GenericRegister

        code.push(Instruction(Op::MOVQ, Arg(index_ptr_generic, IND()), Arg(index_ptr_generic, DIR()), "Unwrap the index pointer"));
        intermediary_storage.push(index_ptr_generic);
    }

    void post_visit(grammar::ast::ArrayIndexAssign &assign) override {
        TargetType value = get_target(pop(intermediary_storage));
        TargetType index_ptr = get_target(pop(intermediary_storage));
        auto index_ptr_generic = std::get<GenericRegister>(index_ptr); // index_ptr is the production of ArrayIndex and thus will always be a GenericRegister
        code.push(Instruction(Op::MOVQ, Arg(value, DIR()), Arg(index_ptr_generic, IND()), "Assign the value to the array index"));
    }

    void post_visit(grammar::ast::PrintStatement &print) override {
        auto target = get_target(pop(intermediary_storage));
        SymbolType type = *print.input_type.get();

        //code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_SAVE, DIR())));
        push_caller_save();
        if (type == BoolType()) {
            code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::RDI, DIR()), "Move value to rdi for print"));
            code.push(Instruction(Op::CALL, Arg(Label("print_bool"), DIR())));
        } else if (type == IntType()) {
            code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::RDI, DIR()), "Move value to rdi for print"));
            code.push(Instruction(Op::CALL, Arg(Label("printNum"), DIR())));
        } else if (boost::get<ClassSymbolType>(&type) != nullptr) {
            code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::RDI, DIR()), "Move value to rdi for print"));
            code.push(Instruction(Op::CALL, Arg(Label("print_object"), DIR())));
        } else if (boost::get<ArraySymbolType>(&type) != nullptr) {
            code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::RDI, DIR()), "Move value to rdi for print"));
            code.push(Instruction(Op::CALL, Arg(Label("print_array"), DIR())));
        } else if (boost::get<BetaType>(&type) != nullptr) {
            code.push(Instruction(Op::CALL, Arg(Label("print_beta"), DIR())));
        } else { // Should not happen since it has been type checked
            throw std::runtime_error("Unsupported type for printing");
        }
        //code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_RESTORE, DIR())));
        push_caller_restore();
    }

    void post_visit(grammar::ast::BreakStatement &break_statement) override {
        grammar::ast::WhileStatement *current_while_loop = while_stack.top();
        std::string end_label = current_while_loop->end_label;
        code.push(Instruction(Op::JMP, Arg(Label(end_label), DIR()), "break statement"));
    }

    void post_visit(grammar::ast::ContinueStatement &continue_statement) override {
        grammar::ast::WhileStatement *current_while_loop = while_stack.top();
        std::string start_label = current_while_loop->start_label;
        code.push(Instruction(Op::JMP, Arg(Label(start_label), DIR()), "continue statement"));
    }

    void pre_visit(grammar::ast::WhileStatement &while_statement) override {
        code.push(Instruction(Op::LABEL, Arg(Label(while_statement.start_label), DIR())));
    }

    void pre_block_visit(grammar::ast::WhileStatement &while_statement) override {
        auto target = get_target(pop(intermediary_storage));
        GenericRegister reg1 = code.new_register();
        code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(reg1, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(1), DIR()), Arg(reg1, DIR())));
        while_stack.push(&while_statement); // push current whileloop on loop stack
        code.push(Instruction(Op::JNE, Arg(Label(while_statement.end_label), DIR())));
    }

    void post_visit(grammar::ast::WhileStatement &while_statement) override {
        pop(while_stack); 
        code.push(Instruction(Op::JMP, Arg(Label(while_statement.start_label), DIR())));
        code.push(Instruction(Op::LABEL, Arg(Label(while_statement.end_label), DIR())));
    }

    void pre_visit(grammar::ast::IfStatement &if_statement) override {
        code.push(Instruction(Op::LABEL, Arg(Label(if_statement.label), DIR())));
    }

    void pre_block_visit(grammar::ast::IfStatement &if_statement) override {
        auto target = get_target(pop(intermediary_storage));
        GenericRegister reg1 = code.new_register();
        code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(reg1, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(1), DIR()), Arg(reg1, DIR())));
        code.push(Instruction(Op::JNE, Arg(Label(if_statement.next_label), DIR())));
    }

    void post_visit(grammar::ast::IfStatement &if_statement) override {
        code.push(Instruction(Op::JMP, Arg(Label(if_statement.end_if_label), DIR())));
    }

    void pre_visit(grammar::ast::ElseStatement &else_statement) override {
        code.push(Instruction(Op::LABEL, Arg(Label(else_statement.label), DIR())));
    }

    void post_visit(grammar::ast::ConditionalStatement &cond_statement) override {
        code.push(Instruction(Op::LABEL, Arg(Label(cond_statement.end_if_label), DIR())));
    }

    void post_visit(grammar::ast::ObjInst &obj) override {
        auto temp = dynamic_cast<ClassSymbol*>(obj.id.sym)->symbol_table;
        auto attrs = temp->get_var_symbols();

        GenericRegister result_register = code.new_register();

        //                                          -1 because heap memory is 0 inxed, but stack memory is not
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue((attrs.size()-1) * 8), DIR()), Arg(Register::RDI, DIR()), "allocate argument"));
        code.push(Instruction(Op::CALL, Arg(Label("allocate"), DIR()), "allocating space for variables"));
        code.push(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result_register, DIR()), "returning address to result_register")); 
        for (size_t i = 0 ; i < attrs.size() ; ++i) {
            code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(0), DIR()), Arg(result_register, IRL(long(8*i))), "initializing variable " + attrs[i]->var_decl->id.id));
        }
        intermediary_storage.push(result_register); 
    }

    void pre_visit(grammar::ast::Prog &prog) override {
        code.push(Instruction(Op::LABEL, Arg(Label("_start"), DIR())));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RBP, DIR()), "save old rbp"));
        code.push(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RBP, DIR()), "set rbp for global scope")); // set rbp
        push_callee_save();
        auto var_decls = global_scope->get_var_symbols();

        for (size_t i = 0; i < var_decls.size(); i++) {
            var_decls[i]->ir_data.is_local = false;
            auto &data = var_decls[i]->ir_data;
            data.stack_offset = code.new_stack_slot();
            code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()), "initialize variable referenced from other scopes to 0"));
        }
    }

    void push_print_function() {
        code.new_empty_scope();
        std::string convert_loop_label = ".LprintNum_convertLoop";
        std::string print_loop_label = ".LprintNum_printLoop";
        std::string print_new_line_label = ".print_newline";
        code.push(Instruction(Op::LABEL, Arg(Label("printNum"), DIR())));
        push_callee_save();

        code.push(Instruction(Op::MOVQ, Arg(Register::RDI, DIR()), Arg(Register::RAX, DIR()), "The number"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(0), DIR()), Arg(Register::R9, DIR()), "Counter for chars to write"));
        code.push(Instruction(Op::LABEL, Arg(Label(convert_loop_label), DIR())));
        code.push(Instruction(Op::NOTHING, "Convert the number to chars"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(0), DIR()), Arg(Register::RDX, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(10), DIR()), Arg(Register::RCX, DIR())));
        code.push(Instruction(Op::IDIVQ, Arg(Register::RCX, DIR())));
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(48), DIR()), Arg(Register::RDX, DIR()), "'0' is 48"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RDX, DIR())));
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(1), DIR()), Arg(Register::R9, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(0), DIR()), Arg(Register::RAX, DIR())));
        code.push(Instruction(Op::JNE, Arg(Label(convert_loop_label), DIR())));
        code.push(Instruction(Op::LABEL, Arg(Label(print_loop_label), DIR())));
        code.push(Instruction(Op::NOTHING, "Print the number to stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "sys_write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "stdout"));
        code.push(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RSI, DIR()), "buf"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDX, DIR()), "len"));
        code.push(Instruction(Op::SYSCALL));
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(8), DIR()), Arg(Register::RSP, DIR()), "len"));
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(-1), DIR()), Arg(Register::R9, DIR()), "len"));
        code.push(Instruction(Op::JNE, Arg(Label(print_loop_label), DIR())));
        code.push(Instruction(Op::NOTHING, "Print newline"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "sys_write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("newline"), DIR()), Arg(Register::RSI, DIR()), "buf"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDX, DIR()), "len"));
        code.push(Instruction(Op::SYSCALL));

        push_callee_restore();
        code.push(Instruction(Op::RET));
        code.end_scope();
    }

    void push_print_bool_function() {
        code.new_empty_scope();
        std::string print_false_label = ".print_bool_false";
        code.push(Instruction(Op::LABEL, Arg(Label("print_bool"), DIR())));
        push_callee_save();
        
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(0), DIR()), Arg(Register::RDI, DIR())));
        code.push(Instruction(Op::JE, Arg(Label(print_false_label), DIR())));

        // Print true
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "System call number for write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "File descriptor for stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("true"), DIR()), Arg(Register::RSI, DIR()), "Address of 'true'"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(5), DIR()), Arg(Register::RDX, DIR()), "Length of string to print"));
        code.push(Instruction(Op::SYSCALL));
        push_callee_restore();
        code.push(Instruction(Op::RET));

        // Print false
        code.push(Instruction(Op::LABEL, Arg(Label(print_false_label), DIR())));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "System call number for write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "File descriptor for stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("false"), DIR()), Arg(Register::RSI, DIR()), "Address of 'false'"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(6), DIR()), Arg(Register::RDX, DIR()), "Length of string to print"));
        code.push(Instruction(Op::SYSCALL));
        push_callee_restore();
        code.push(Instruction(Op::RET));
        code.end_scope();
    }

    void push_print_object_function() {
        code.new_empty_scope();
        std::string print_null_label = ".print_object_null";
        code.push(Instruction(Op::LABEL, Arg(Label("print_object"), DIR())));
        push_callee_save();

        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(0), DIR()), Arg(Register::RDI, DIR())));
        code.push(Instruction(Op::JE, Arg(Label(print_null_label), DIR())));

        // Print object
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "System call number for write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "File descriptor for stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("object"), DIR()), Arg(Register::RSI, DIR()), "Address of 'object'"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(6), DIR()), Arg(Register::RDX, DIR()), "Length of string to print"));
        code.push(Instruction(Op::SYSCALL));
        push_callee_restore();
        code.push(Instruction(Op::RET));

        // Print beta
        code.push(Instruction(Op::LABEL, Arg(Label(print_null_label), DIR())));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "System call number for write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "File descriptor for stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("beta"), DIR()), Arg(Register::RSI, DIR()), "Address of 'beta'"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(4), DIR()), Arg(Register::RDX, DIR()), "Length of string to print"));
        code.push(Instruction(Op::SYSCALL));
        push_callee_restore();
        code.push(Instruction(Op::RET));
        code.end_scope();
    }

    void push_print_array_function() {
        code.new_empty_scope();
        std::string print_null_label = ".print_array_null";
        code.push(Instruction(Op::LABEL, Arg(Label("print_array"), DIR())));
        push_callee_save();

        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(0), DIR()), Arg(Register::RDI, DIR())));
        code.push(Instruction(Op::JE, Arg(Label(print_null_label), DIR())));

        // Print object
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "System call number for write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "File descriptor for stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("array"), DIR()), Arg(Register::RSI, DIR()), "Address of 'array'"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(5), DIR()), Arg(Register::RDX, DIR()), "Length of string to print"));
        code.push(Instruction(Op::SYSCALL));
        push_callee_restore();
        code.push(Instruction(Op::RET));

        // Print beta
        code.push(Instruction(Op::LABEL, Arg(Label(print_null_label), DIR())));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "System call number for write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "File descriptor for stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("beta"), DIR()), Arg(Register::RSI, DIR()), "Address of 'beta'"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(4), DIR()), Arg(Register::RDX, DIR()), "Length of string to print"));
        code.push(Instruction(Op::SYSCALL));
        push_callee_restore();
        code.push(Instruction(Op::RET));
        code.end_scope();
    }

    void push_print_beta_function() {
        code.new_empty_scope();
        code.push(Instruction(Op::LABEL, Arg(Label("print_beta"), DIR())));

        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "System call number for write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "File descriptor for stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("beta"), DIR()), Arg(Register::RSI, DIR()), "Address of 'beta'"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(4), DIR()), Arg(Register::RDX, DIR()), "Length of string to print"));
        code.push(Instruction(Op::SYSCALL));

        code.push(Instruction(Op::RET));
        code.end_scope();
    }

    void push_mem_alloc_function() {
        code.new_empty_scope();
        code.push(Instruction(Op::LABEL, Arg(Label("allocate"), DIR())));
        code.push(Instruction(Op::PUSHQ , Arg(Register::RBP, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RBP, DIR())));
        push_caller_save();

        code.push(Instruction(Op::PUSHQ, Arg(Register::RDI, DIR())));
        code.push(Instruction(Op::NOTHING, "1. Find the current end of the data segment."));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(12), DIR()), Arg(Register::RAX, DIR()), "brk"));
        code.push(Instruction(Op::XORQ, Arg(Register::RDI, DIR()), Arg(Register::RDI, DIR()), "0 means we retrieve the current end"));
        code.push(Instruction(Op::SYSCALL));
        code.push(Instruction(Op::NOTHING, "2. Add the amount of memory we want to allocate."));
        code.push(Instruction(Op::POPQ, Arg(Register::RDI, DIR()), "the argument"));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RAX, DIR()), "current end, which is where the allocated memory will start"));
        code.push(Instruction(Op::ADDQ, Arg(Register::RAX, DIR()), Arg(Register::RDI, DIR()), "compute the new end"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(12), DIR()), Arg(Register::RAX, DIR()), "brk"));
        code.push(Instruction(Op::SYSCALL));
        GenericRegister result = code.new_register();
        code.push(Instruction(Op::POPQ, Arg(result, DIR()), "the old end, which is the address of our allocated memory"));

        push_caller_restore();
        code.push(Instruction(Op::MOVQ, Arg(result, DIR()), Arg(Register::RAX, DIR())));

        code.push(Instruction(Op::POPQ, Arg(Register::RBP, DIR())));
        
        code.push(Instruction(Op::RET));
        code.end_scope();
    }

    void push_print_is_beta_function() {
        code.new_empty_scope();
        code.push(Instruction(Op::LABEL, Arg(Label("print_is_beta"), DIR())));
        push_callee_save();

        code.push(Instruction(Op::PUSHQ, Arg(Register::RDI, DIR()), "Push line number"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR()), "System call number for write"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR()), "File descriptor for stdout"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateData("is_beta"), DIR()), Arg(Register::RSI, DIR()), "Address of string"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(41), DIR()), Arg(Register::RDX, DIR()), "Length of string to print"));
        code.push(Instruction(Op::SYSCALL));
        // Print line number
        code.push(Instruction(Op::POPQ, Arg(Register::RDI, DIR()), "Pop line number"));
        push_caller_save();
        code.push(Instruction(Op::CALL, Arg(Label("printNum"), DIR())));
        push_caller_restore();

        // Close with error code 1
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(60), DIR()), Arg(Register::RAX, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RDI, DIR())));
        code.push(Instruction(Op::SYSCALL));

        code.end_scope();
    }


    void push_standard_functions() {
        push_print_function();
        push_mem_alloc_function();
        push_print_bool_function();
        push_print_object_function();
        push_print_array_function();
        push_print_beta_function();
        push_print_is_beta_function();
    }

    void post_visit(grammar::ast::Prog &prog) override {
        code.push(Instruction(Op::PUSHQ, Arg(Register::RBP, DIR()), "setting static link")); // Settting static link.
        code.push(Instruction(Op::CALL, Arg(Label("main"), DIR())));
        code.push(Instruction(Op::POPQ, Arg(Register::RBP, DIR()), "remove static link from stack"));
        // code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_RESTORE, DIR())));
        push_callee_restore();
        code.push(Instruction(Op::POPQ, Arg(Register::RBP, DIR()), "restore old rbp"));
        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(60), DIR()), Arg(Register::RAX, DIR())));
        code.push(Instruction(Op::XORQ, Arg(Register::RDI, DIR()), Arg(Register::RDI, DIR())));
        code.push(Instruction(Op::SYSCALL));
        
        push_standard_functions();
    }

    template<typename T>
    T pop(std::stack<T>& my_stack) {
        if (my_stack.empty()) {
            throw std::runtime_error("Attempting to pop from an empty stack");
        }
        T top_element = std::move(my_stack.top());
        my_stack.pop();
        return top_element;
    }

};

IR *intermediate_code_generation(grammar::ast::Prog &prog) {
    auto visitor = IRVisitor(prog);
    visitor.code.new_scope(prog.global_scope); // add global scope
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    visitor.code.end_scope(); // end global scope
    IR *funcs = visitor.code.get_instructions();
    link_instructions(*funcs);
    return funcs;
}


