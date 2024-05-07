#include "code_generation.hpp"
#include "../semantics/symbol_table.hpp"
#include "../visitor.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <stack>

const int callee_offset = -40;

using AstValue = std::variant<int, bool, GenericRegister>;

class FunctionOrderManager {
private:
    std::stack<size_t> current_function_index;
    std::vector<std::vector<Instruction>> list_of_funcs;
public:
    FunctionOrderManager();
    void new_scope();
    void end_scope();
    void push(Instruction Instruction);
    IR get_instructions();
};

/// Expects there to be space on the stack for the result register taken as input. 
/// uses register R8 and R9, so should be saved before use
std::vector<Instruction> static_link_read(int depth, int target_local_id, GenericRegister read_result) {
    std::vector<Instruction> instructions;
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR()), "starting static linking read"));
    // std::cout << "depth: " << depth << std::endl;
    for (auto i = 0; i < depth; i++) {
        instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, IRL(16)), Arg(Register::R9, DIR())));
        instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
    }
    GenericRegister target = GenericRegister(target_local_id);
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R9, DIR()), "save RBP")); // save RBP
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RBP, DIR()))); // set RBP to R8, so generic register points to correct memory location.
    instructions.push_back(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::R8, DIR()), "temporarely save result"));
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::RBP, DIR()), "restore RBP")); // restore RBP
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(read_result, DIR()), "move result to result register"));
    return instructions;
}

/// uses register R8, R9 and R10, so should be saved before use
std::vector<Instruction> static_link_write(int depth, int target_local_id, TargetType write_value) {
    std::vector<Instruction> instructions;
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR()), "starting static linking write"));
    // std::cout << "depth: " << depth << std::endl;
    for (auto i = 0; i < depth; i++) {
        instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, IRL(16)), Arg(Register::R9, DIR())));
        instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
    }
    GenericRegister target = GenericRegister(target_local_id);

    instructions.push_back(Instruction(Op::MOVQ, Arg(write_value, DIR()), Arg(Register::R10, DIR()), "put write value in temp storage")); // save RBP
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R9, DIR()), "save RBP")); // save RBP
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RBP, DIR()))); // set RBP to R8, so generic register points to correct memory location.
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(target, DIR()), "assign value to target"));
    instructions.push_back(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::RBP, DIR()), "restore RBP")); // restore RBP
    return instructions;
}

std::vector<Instruction> binop_instructions(std::string op, GenericRegister result){
    std::vector<Instruction> code;
    if (op == "+") {
        code.push_back(Instruction(Op::ADDQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "-") {
        code.push_back(Instruction(Op::SUBQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "*") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::IMULQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "/") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RAX, DIR())));
        code.push_back(Instruction(Op::IDIVQ, Arg(Register::R9, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR())));
    } else if (op == "%") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::RDX, DIR()), Arg(Register::RDX, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RAX, DIR())));
        code.push_back(Instruction(Op::IDIVQ, Arg(Register::R9, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::RDX, DIR()), Arg(result, DIR())));
    } else if (op == "&") {
        code.push_back(Instruction(Op::ANDQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "|") {
        code.push_back(Instruction(Op::ORQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(result, DIR())));
    } else if (op == "<") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETL, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == ">") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETG, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "==") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "!=") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETNE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == "<=") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETLE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    } else if (op == ">=") {
        code.push_back(Instruction(Op::XORQ, Arg(Register::R10, DIR()), Arg(Register::R10, DIR())));
        code.push_back(Instruction(Op::CMPQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        code.push_back(Instruction(Op::SETGE, Arg(Register::R10B, DIR())));
        code.push_back(Instruction(Op::MOVQ, Arg(Register::R10, DIR()), Arg(result, DIR())));
    }
    return code;
}

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
    FunctionOrderManager code;

    SymbolTable* global_scope;
    std::stack<grammar::ast::WhileStatement*> while_stack;

    IRVisitor(grammar::ast::Prog &prog) : Visitor(), global_scope(prog.global_scope) {}

private:
    std::vector<std::string> function_container;
    std::stack<AstValue> temp_storage;


    void post_visit(grammar::ast::ReturnStatement &return_statement) {
        //AstValue value = pop(temp_storage);
        auto target = get_target(pop(temp_storage));
        code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::RAX, DIR())));

        // Remove local variables from the stack
        code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(callee_offset), DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(Register::RSP, DIR())));

        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_RESTORE, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::RSP, DIR())));
        code.push(Instruction(Op::POPQ, Arg(Register::RBP, DIR())));
        code.push(Instruction(Op::RET));
    }

    void pre_visit(grammar::ast::FuncDecl &func_decl) {
        code.new_scope();
        code.push(Instruction(Op::LABEL, Arg(Label(func_decl.label), DIR())));
        code.push(Instruction(Op::PUSHQ, Arg(Register::RBP, DIR()), "save old rbp"));
        code.push(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RBP, DIR()), "set rbp for function scope"));
        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_SAVE, DIR())));
        std::vector<VarSymbol*> var_decls = func_decl.sym->sym_tab->get_var_symbols();
        for (size_t i = 0; i < var_decls.size(); i++) {
            code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()), "initialize local variable to 0"));
        }
    }

    void post_visit(grammar::ast::FuncDecl &func_decl) {
        code.end_scope();
    }

    void post_visit(grammar::ast::FunctionCall &func_call) {
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()), "make space on stack")); // make space on stack for generic register value
        GenericRegister result = GenericRegister(++func_call.id.scope->register_counter); // register for the function result to be stored in

        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_SAVE, DIR())));

        // add arguments to stack in reverse order
        for (long unsigned int i = 0; i < func_call.argument_list.arguments.size(); i++) {
            //AstValue value = pop(temp_storage);
            auto target = get_target(pop(temp_storage));
            code.push(Instruction(Op::PUSHQ, Arg(target, DIR()), "pushing register argument"));
        }
        int callee_depth = dynamic_cast<FuncSymbol*>(func_call.id.sym)->sym_tab->parent_scope->depth;
        int caller_depth = func_call.id.scope->depth;
        int difference = caller_depth - callee_depth;
        
        code.push(Instruction(Op::MOVQ, Arg(Register::RBP, DIR()), Arg(Register::R8, DIR()), "calculating static link for function call"));
        for (auto i = 0; i < difference; i++) {
            code.push(Instruction(Op::MOVQ, Arg(Register::R8, IRL(16)), Arg(Register::R9, DIR())));
            code.push(Instruction(Op::MOVQ, Arg(Register::R9, DIR()), Arg(Register::R8, DIR())));
        } 

        code.push(Instruction(Op::PUSHQ, Arg(Register::R8, DIR()), "setting static link")); // Settting static link.
        std::string label = dynamic_cast<FuncSymbol*>(func_call.id.sym)->func_decl->label;
        // std::cout << label << std::endl;
        code.push(Instruction(Op::CALL, Arg(Label(label), DIR())));
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue((func_call.argument_list.arguments.size()+1) * 8), DIR()), Arg(Register::RSP, DIR()), "remove arguments and static link from stack")); 
        code.push(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(result, DIR()), "save result from function call")); 
        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLER_RESTORE, DIR())));
        temp_storage.push(result); // pushing the result of the function
    }

    VarSymbol *get_var_symbols(Symbol *symbol) {
        if (auto var_symbol = dynamic_cast<VarSymbol *>(symbol)) {
            return var_symbol;
        } else {
            throw IRError("Attempted to use a symbol as a VarSymbol, despite not being one");
        }
    }

    void post_visit(grammar::ast::VarAssign &var_assign) {
      //AstValue value = pop(temp_storage);
      auto target = get_target(pop(temp_storage));
      VarSymbol *var_symbol = get_var_symbols(var_assign.id_access.ids.back().sym);
      int current_depth = var_assign.id_access.ids.back().scope->depth;
      int target_depth = var_symbol->var_decl->id.scope->depth;
      int difference = current_depth - target_depth;
      int local_id = var_symbol->local_id;
      auto static_linking_code = static_link_write(difference, local_id, target);
      for (auto instruction : static_linking_code) {
          code.push(instruction);
      }
    }

    void post_visit(grammar::ast::VarDeclAssign &var_decl_assign) {
        auto target = get_target(pop(temp_storage));
        code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(Register::R8, DIR()), Arg(GenericRegister(var_decl_assign.decl.sym->local_id), DIR())));
    }


    void pre_visit(int &i) {
        temp_storage.push(i);
    }

    void pre_visit(bool &b) {
        temp_storage.push(b);
    }

    void post_visit(grammar::ast::VarExpression &var_expr) {
        VarSymbol *var_symbol = dynamic_cast<VarSymbol*>(var_expr.id_access.ids.back().sym);
        auto target_depth = var_symbol->var_decl->id.scope->depth;
        int current_depth = var_expr.id_access.ids.back().scope->depth;
        int difference = current_depth - target_depth;
        // std::cout << "current depth: " << current_depth << "target depth: " << target_depth << "difference: " << difference << std::endl;
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
        auto id = ++var_expr.id_access.ids.back().scope->register_counter;
        GenericRegister result_register = GenericRegister(id);
        auto static_linking_code = static_link_read(difference, var_symbol->local_id, result_register);
        for (auto instruction : static_linking_code) {
            code.push(instruction);
        }
        temp_storage.push(result_register);
    }

    void post_visit(grammar::ast::Rhs &op_exp) {
        // future optimization: calculate immediate values immediately to optimize program.
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register
        GenericRegister result = GenericRegister(++op_exp.scope->register_counter);

        auto r_target = get_target(pop(temp_storage));
        auto l_target = get_target(pop(temp_storage));

        code.push(Instruction(Op::MOVQ, Arg(l_target, DIR()), Arg(Register::R8, DIR())));
        code.push(Instruction(Op::MOVQ, Arg(r_target, DIR()), Arg(Register::R9, DIR())));

        auto binop_result = binop_instructions(op_exp.op, result);
        for (auto instruction : binop_result) {
            code.push(instruction);
        }

        temp_storage.push(result);  
    }

    void post_visit(grammar::ast::ArrayInitExp &arr) {
        std::vector<AstValue> sizes;
        for (auto size : arr.sizes) {
            sizes.push_back(pop(temp_storage));
        }

        TargetType memSize = Register::RDI;

        code.push(Instruction(Op::PUSHQ, Arg(memSize, DIR()), "Save that shit"));


        code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(8), DIR()), Arg(memSize, DIR()), "initialize memory size" ));
        for (auto value : sizes) {
            code.push(Instruction(Op::IMULQ, Arg(get_target(value), DIR()), Arg(memSize, DIR()), "calculate memory"));
        }
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(arr.sizes.size() * 8), DIR()), Arg(memSize, DIR()), "found size of memory"));
        
        // allocate memory
        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::MEM_ALLOC, DIR()), Arg(memSize, DIR()), "allocate memory of found memory size"));
        
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
        GenericRegister arrayStart = GenericRegister(++arr.scope->register_counter);
        code.push(Instruction(Op::MOVQ, Arg(Register::RAX, DIR()), Arg(arrayStart, DIR()), "Save array pointer in generic register" ));

        // Set the size of each dimension of  the array
        for (size_t i = 0; i < sizes.size(); i++) {
            code.push(Instruction(Op::MOVQ, Arg(get_target(sizes[i]), DIR()), Arg(arrayStart, IRL(i * 8)), "set size of dimension " + std::to_string(i + 1)));
        }
        // set the array pointer to point to the first element of the array
        code.push(Instruction(Op::ADDQ, Arg(ImmediateValue(sizes.size() * 8), DIR()), Arg(arrayStart, DIR()), "set array pointer to point to first element" ));

        code.push(Instruction(Op::POPQ, Arg(memSize, DIR()), "pop that shit"));
        
        temp_storage.push(arrayStart);
    }


    void post_visit(grammar::ast::ArrayIndex &index) {
        std::vector<TargetType> index_targets;
        for (size_t i = 0; i < index.indices.size(); i++) {
            index_targets.push_back(get_target(pop(temp_storage)));
        }

        // TODO: Check valid index
        // TODO: Make sure arrays are also 0-initialized at the beginning of their scope

        // Static link
        SymbolTable *scope = index.id_access.ids.front().scope;
        VarSymbol *target_symbol = get_var_symbols(index.id_access.ids.back().sym);
        int target_depth = target_symbol->var_decl->id.scope->depth;
        int current_depth = scope->depth;
        int difference = current_depth - target_depth;
        // std::cout << "current depth: " << current_depth << "target depth: " << target_depth << "difference: " << difference << std::endl;
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
        GenericRegister array_ptr = GenericRegister(++scope->register_counter);
        auto static_linking_code = static_link_read(difference, target_symbol->local_id, array_ptr);
        for (auto instruction : static_linking_code) {
            code.push(instruction);
        }


        // Get a pointer to the correct index
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
        GenericRegister index_address = GenericRegister(++scope->register_counter);
        GenericRegister intermediate_product = GenericRegister(++scope->register_counter);
        GenericRegister intermediate_value = GenericRegister(++scope->register_counter);
        // NOTE: indexTargets are in reverse order such that the last index is first
        code.push(Instruction(Op::MOVQ, Arg(array_ptr, DIR()), Arg(index_address, DIR()), "Initialize the index address"));
        code.push(Instruction(Op::MOVQ, Arg(index_targets.front(), DIR()), Arg(intermediate_value, DIR()), "Initialize the intermediate value"));
        code.push(Instruction(Op::IMULQ, Arg(ImmediateValue(8), DIR()), Arg(intermediate_value, DIR()), "Translate the first index to byte address"));
        code.push(Instruction(Op::ADDQ, Arg(intermediate_value, DIR()), Arg(index_address, DIR()), "Add the first index to the address"));

        // Multi-dimensional array indexing
        if (index.indices.size() > 1) {
            code.push(Instruction(Op::MOVQ, Arg(ImmediateValue(1), DIR()), Arg(intermediate_product, DIR()), "Initialize the product"));
            for (size_t i = 1; i < index.indices.size(); i++) {
                code.push(Instruction(Op::IMULQ, Arg(array_ptr, IRL(-i * 8)), Arg(intermediate_product, DIR()), "Multiply with the previous product"));
                code.push(Instruction(Op::MOVQ, Arg(index_targets[i], DIR()), Arg(intermediate_value, DIR()), "Initialize the intermediate value"));
                code.push(Instruction(Op::IMULQ, Arg(intermediate_product, DIR()), Arg(intermediate_value, DIR()), "Multiply the intermediate value and product"));
                code.push(Instruction(Op::IMULQ, Arg(ImmediateValue(8), DIR()), Arg(intermediate_value, DIR()), "Translate the intermediate value to a byte address"));
                code.push(Instruction(Op::ADDQ, Arg(intermediate_value, DIR()), Arg(index_address, DIR()), "Add the intermediate value to the address"));
            }
        }

        temp_storage.push(index_address);
    }



    void post_visit(grammar::ast::ArrayIndexExp &index_exp) {
        TargetType index_ptr = get_target(pop(temp_storage));
        code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()))); // make space on stack for generic register value
        GenericRegister result = GenericRegister(++index_exp.index.id_access.ids.front().scope->register_counter);
        code.push(Instruction(Op::MOVQ, Arg(index_ptr, IND()), Arg(result, DIR()), "Unwrap the index pointer"));
        temp_storage.push(result);
    }

    void post_visit(grammar::ast::ArrayIndexAssign &assign) {
        TargetType value = get_target(pop(temp_storage));
        TargetType index_ptr = get_target(pop(temp_storage));
        code.push(Instruction(Op::MOVQ, Arg(value, DIR()), Arg(index_ptr, IND()), "Assign the value to the array index"));
    }

    void post_visit(grammar::ast::PrintStatement &print) {
        auto target = get_target(pop(temp_storage));
        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::PRINT, DIR()), Arg(target, DIR())));
    }

    void post_visit(grammar::ast::BreakStatement &break_statement) {
        grammar::ast::WhileStatement *current_while_loop = while_stack.top();
        std::string end_label = current_while_loop->end_label;
        code.push(Instruction(Op::JMP, Arg(Label(end_label), DIR())));
    }

    void post_visit(grammar::ast::ContinueStatement &continue_statement) {
        grammar::ast::WhileStatement *current_while_loop = while_stack.top();
        std::string start_label = current_while_loop->start_label;
        code.push(Instruction(Op::JMP, Arg(Label(start_label), DIR())));
    }

    void pre_visit(grammar::ast::WhileStatement &while_statement) {

        code.push(Instruction(Op::LABEL, Arg(Label(while_statement.start_label), DIR())));
    }

    void pre_block_visit(grammar::ast::WhileStatement &while_statement) {
        auto target = get_target(pop(temp_storage));
        code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::RAX, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR())));
        while_stack.push(&while_statement); // push current whileloop on loop stack
        code.push(Instruction(Op::JNE, Arg(Label(while_statement.end_label), DIR())));
    }

    void post_visit(grammar::ast::WhileStatement &while_statement) {
        pop(while_stack); 
        code.push(Instruction(Op::JMP, Arg(Label(while_statement.start_label), DIR())));
        code.push(Instruction(Op::LABEL, Arg(Label(while_statement.end_label), DIR())));
    }

    void pre_visit(grammar::ast::IfStatement &if_statement) {
        code.push(Instruction(Op::LABEL, Arg(Label(if_statement.label), DIR())));
    }

    void pre_block_visit(grammar::ast::IfStatement &if_statement) {
        auto target = get_target(pop(temp_storage));
        code.push(Instruction(Op::MOVQ, Arg(target, DIR()), Arg(Register::RAX, DIR())));
        code.push(Instruction(Op::CMPQ, Arg(ImmediateValue(1), DIR()), Arg(Register::RAX, DIR())));
        code.push(Instruction(Op::JNE, Arg(Label(if_statement.next_label), DIR())));
    }

    void post_visit(grammar::ast::IfStatement &if_statement) {
        code.push(Instruction(Op::JMP, Arg(Label(if_statement.end_if_label), DIR())));
    }

    void pre_visit(grammar::ast::ElseStatement &else_statement) {
        code.push(Instruction(Op::LABEL, Arg(Label(else_statement.label), DIR())));
    }

    void post_visit(grammar::ast::ConditionalStatement &cond_statement) {
        code.push(Instruction(Op::LABEL, Arg(Label(cond_statement.end_if_label), DIR())));
    }


    void pre_visit(grammar::ast::Prog &prog) {
        code.push(Instruction(Op::MOVQ, Arg(Register::RSP, DIR()), Arg(Register::RBP, DIR()), "set rbp for global scope")); // set rbp
        code.push(Instruction(Op::PROCEDURE, Arg(Procedure::CALLEE_SAVE, DIR())));
        // std::cout << global_scope->get_var_symbols().size() << std::endl;
        int var_count = global_scope->get_var_symbols().size();
        for (int i = 0; i < var_count; i++) {
            code.push(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()), "initialize global variable to 0"));
        }
    }

    void push_print_function() {
        std::string convert_loop_label = ".LprintNum_convertLoop";
        std::string print_loop_label = ".LprintNum_printLoop";
        std::string print_new_line_label = ".print_newline";
        code.push(Instruction(Op::LABEL, Arg(Label("printNum"), DIR())));
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
        code.push(Instruction(Op::RET));

    }

    void push_mem_alloc_function() {
        code.push(Instruction(Op::LABEL, Arg(Label("allocate"), DIR())));
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
        code.push(Instruction(Op::POPQ, Arg(Register::RAX, DIR()), "the old end, which is the address of our allocated memory"));
        code.push(Instruction(Op::RET));
    }

    void push_standard_functions() {
        push_print_function();
        push_mem_alloc_function();
    }

    void post_visit(grammar::ast::Prog &prog) {
        code.push(Instruction(Op::PUSHQ, Arg(Register::RBP, DIR()), "setting static link")); // Settting static link.
        code.push(Instruction(Op::CALL, Arg(Label("main"), DIR())));
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

IR intermediate_code_generation(grammar::ast::Prog &prog) {
    
    auto visitor = IRVisitor(prog);
    visitor.code.new_scope(); // add global scope
    auto traveler = TreeTraveler(visitor);
    traveler(prog);
    visitor.code.end_scope(); // end global scope
    return visitor.code.get_instructions();
}


FunctionOrderManager::FunctionOrderManager() : current_function_index(), list_of_funcs() {}

/// adds a new list of instructions to the current scope
void FunctionOrderManager::new_scope() {
    list_of_funcs.push_back(std::vector<Instruction>());
    current_function_index.push(list_of_funcs.size()-1);
}

/// pushes an instruction to the current scope
void FunctionOrderManager::push(Instruction instruction) {
    // auto l = current_function_index.top();
    // std::cout << "l: " << l << std::endl;
    // std::cout << "func: " << list_of_funcs[l] << std::endl;
    list_of_funcs[current_function_index.top()].push_back(instruction);
}

/// pops the current scope
void FunctionOrderManager::end_scope() {
    current_function_index.pop();
}

/// get all the instructions in preorder.
IR FunctionOrderManager::get_instructions() {
    IR instructions;
    for (auto &func : list_of_funcs) {
        instructions.insert(instructions.end(), func.begin(), func.end());
    }
    return instructions;
}

