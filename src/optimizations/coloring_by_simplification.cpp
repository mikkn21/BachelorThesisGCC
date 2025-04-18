#include "../IR/ir.hpp"
#include "control_flow_graph.hpp"
#include "liveness_analysis.hpp"
#include "graph.hpp"
#include <stack>
#include <iostream>


const int callee_offset = -48; // TODO: this should be referenced from a shared file so all occurences of this has a common variable. Or it should be removed
const int arg_offset = 16;
const std::array<Register, 14> REGISTERS = {Register::RAX, Register::RBX, Register::RCX, Register::RDX, Register::RDI, Register::RSI, Register::R8, Register::R9, Register::R10, Register::R11, Register::R12, Register::R13, Register::R14, Register::R15};



Graph<RegisterType> build_interference_graph(LivenessAnalysis &blocks) {
    Graph<RegisterType> graph;
    // std::cout << "building interference graph ------------" << std::endl;
    for (const auto block : blocks) {
        // std::cout << "\nblock in building inference graph:" << std::endl;
        for (const auto reg : block->def) {
            graph.add_vertex(reg);
        }
        for (const auto reg : block->in) {
            graph.add_vertex(reg);
        }
        for (const auto reg : block->out) {
            graph.add_vertex(reg);
        }

        for (const auto &def_var : block->def) {
            // std::cout << "def_var" << std::endl;
            for (const auto &out_var : block->out) {
                graph.add_edge(def_var, out_var);
            }
        }
    }

    return std::move(graph);
}

std::tuple<std::set<GenericRegister>, std::set<GenericRegister>> make_worklist(Graph<RegisterType> &graph) {
    std::set<GenericRegister> simplify_worklist;
    std::set<GenericRegister> spill_worklist;
    std::set<GenericRegister> initial_worklist;

    for (const auto &vertex : graph.get_vertices()) {
        if (std::holds_alternative<GenericRegister>(vertex.value)) {
            initial_worklist.insert(std::get<GenericRegister>(vertex.value));
        }
    }

    for (const auto &vertex : initial_worklist) {
        if (graph.degree(vertex) >= REGISTERS.size()) {
            spill_worklist.insert(vertex);
        } else {
            simplify_worklist.insert(vertex);
        }
    }

    return std::make_tuple(simplify_worklist, spill_worklist);
}

/// Precondition: simplify_worklist is not empty
void simplify(Graph<RegisterType> &graph, std::set<GenericRegister> &spill_worklist, std::set<GenericRegister> &simplify_worklist, std::stack<GenericRegister> &select_stack) {
    GenericRegister node = *simplify_worklist.begin();
    simplify_worklist.erase(node);
    select_stack.push(node);

    for (const auto &neighbor : graph.get_neighbors(node)) {
        if (graph.degree(neighbor->value) == REGISTERS.size()) {
            if (std::holds_alternative<GenericRegister>(neighbor->value)) {
                const auto &generic_neighbor = std::get<GenericRegister>(neighbor->value);
                spill_worklist.erase(generic_neighbor);
                simplify_worklist.insert(generic_neighbor);
            }

        }
        auto node_reg_type = RegisterType(node);
        graph.remove_edge(node_reg_type, neighbor->value);
    }
}

void select_spill(Graph<RegisterType> &graph, std::set<GenericRegister> &spill_worklist, std::set<GenericRegister> &simplify_worklist) {
    GenericRegister node = *spill_worklist.begin();
    
    
    spill_worklist.erase(node);
    simplify_worklist.insert(node);
}

std::set<Register> get_okay_colors() {
    std::set<Register> ok_colors;
    for (const auto &reg : REGISTERS) {
        ok_colors.insert(reg);
    }
    return ok_colors;
}

void assign_colors(Graph<RegisterType> &graph, std::stack<GenericRegister> &select_stack, std::set<GenericRegister> &spilled_nodes, std::set<GenericRegister> &colored_nodes, std::map<GenericRegister, Register> &color_mapping) {
    while (!select_stack.empty()) {
        GenericRegister node = select_stack.top();
        select_stack.pop();
        auto ok_colors = get_okay_colors();
        for (const auto &neighbor : graph.get_neighbors(node)) {
            if (std::holds_alternative<Register>(neighbor->value)) {
                auto neighbor_reg = std::get<Register>(neighbor->value);
                ok_colors.erase(neighbor_reg);
            } else if (std::holds_alternative<GenericRegister>(neighbor->value) && colored_nodes.find(std::get<GenericRegister>(neighbor->value)) != colored_nodes.end()) {
                ok_colors.erase(color_mapping[std::get<GenericRegister>(neighbor->value)]);
            }
        }
        if (ok_colors.empty()) {
            spilled_nodes.insert(node);
        } else {
            colored_nodes.insert(node);
            color_mapping[node] = *ok_colors.begin();
        }
    }
}

bool instruction_is_callee_save_end(const Instruction &instruction) {
    return instruction.comment.value_or("") == "END OF CALLEE SAVE";
}

bool block_has_callee_save_end(Block &block) {
    for (const auto &instruction : block.instructions) {
        if (instruction_is_callee_save_end(instruction)) {
            return true;
        }
    }
    return false;
}

auto skip_activation_record(LivenessAnalysis &blocks) {
    auto blocks_iter = blocks.begin();
    while (blocks_iter != blocks.end() && !block_has_callee_save_end(**blocks_iter)) {
        auto instructions = (*blocks_iter)->instructions;
        ++blocks_iter;
    }
    ++blocks_iter;
    return blocks_iter;
}

auto skip_activation_record_instructions(std::list<Instruction> &code) {
    auto it = code.begin();
    while (it != code.end() && !instruction_is_callee_save_end(*it)) {
        ++it;
    }
    ++it;
    return it;
}

void rewrite_program(Function &func, LivenessAnalysis &blocks, std::set<GenericRegister> &spill_worklist, std::set<GenericRegister> &colored_nodes, std::set<GenericRegister> spilled_nodes, std::map<GenericRegister, Register> &color_mapping) {
    std::map<GenericRegister, long> stack_mapping;
    auto blocks_iter = skip_activation_record(blocks);
    auto code_iter = skip_activation_record_instructions(func.code);

    for (const auto &spilled_node : spilled_nodes) {
        func.code.insert(code_iter, Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()), "makeing space for spilled node"));
        // std::cout << "spilled node: " << spilled_node.local_id << std::endl;
        // std::cout << "offset: " << (func.get_stack_counter() + static_cast<long>(stack_mapping.size()))*(-8) + callee_offset << std::endl;
        // std::cout << "func.get_stack_counter(): " << func.get_stack_counter() << std::endl;
        // std::cout << "stack_mapping.size(): " << stack_mapping.size() << std::endl;
        stack_mapping[spilled_node] = func.new_stack_slot()*(-8) + callee_offset;
    }

    while (blocks_iter != blocks.end()) {
        std::map<GenericRegister, GenericRegister> spill_use_mapping;
        std::map<GenericRegister, GenericRegister> spill_def_mapping;

        for (auto &use : (*blocks_iter)->use) {
            if (std::holds_alternative<GenericRegister>(use)) {
                auto generic_use = std::get<GenericRegister>(use);
                if (stack_mapping.find(generic_use) != stack_mapping.end()) {
                    GenericRegister use_reg = func.new_register();
                    spill_use_mapping[generic_use] = use_reg;
                    func.code.insert(code_iter, Instruction(Op::MOVQ, Arg(Register::RBP, IRL(stack_mapping[generic_use])), Arg(use_reg, DIR()), "use register"));
                }
            }
        }        
        for (auto &def : (*blocks_iter)->def) {
            if (std::holds_alternative<GenericRegister>(def)) {
                auto generic_def = std::get<GenericRegister>(def);
                if (stack_mapping.find(generic_def) != stack_mapping.end()) {
                    if (spill_use_mapping.find(generic_def) == spill_use_mapping.end()) {
                        GenericRegister def_reg = func.new_register();
                        spill_def_mapping[generic_def] = def_reg;
                    } else {
                        spill_def_mapping[generic_def] = spill_use_mapping[generic_def];
                    }
                }
            }
        }

        for (size_t i = 0; i < (*blocks_iter)->instructions.size(); ++i) {
            Instruction &instruction = *code_iter;
            for (auto &arg : instruction.args) {
                if (std::holds_alternative<GenericRegister>(arg.target)) {
                    auto generic_arg = std::get<GenericRegister>(arg.target);
                    if (stack_mapping.find(generic_arg) != stack_mapping.end()) {
                        if (spill_use_mapping.find(generic_arg) != spill_use_mapping.end()) {
                            arg.target = spill_use_mapping[generic_arg];
                        } else if (spill_def_mapping.find(generic_arg) != spill_def_mapping.end()) {
                            arg.target = spill_def_mapping[generic_arg];
                        } else {
                            throw std::runtime_error("Generic register not found in spill_use_mapping or spill_def_mapping even though it should have been");
                        }
                    } 
                }
            }
            ++code_iter;
        }

        for (auto def : spill_def_mapping) {
            func.code.insert(code_iter, Instruction(Op::MOVQ, Arg(def.second, DIR()), Arg(Register::RBP, IRL(stack_mapping[def.first])), "def register"));
        }

        ++blocks_iter;
    }

}

void apply_color_mapping(Function &func, std::map<GenericRegister, Register> &color_mapping) {
    for (auto &instruction : func.code) {
        for (auto &arg : instruction.args) {
            if (std::holds_alternative<GenericRegister>(arg.target)) {
                auto generic_arg = std::get<GenericRegister>(arg.target);
                arg.target = color_mapping[generic_arg];
            }
        }
    }
}

// the coloring should happpen after register_allocation_recursive i think
// what data structure should be shared/global and what should be local. color_mapping should be global, and it is local right now.
// right now I have made the mapping from generic register to actual register in the rewrite_program function, but it should be done after the recursive function.
void register_allocation_recursive(IR &ir) {
    for (auto func : ir.functions) {
        // std::cout << "new func register allocation" << std::endl;
        LivenessAnalysis blocks = liveness_analysis(func->code);
        // std::cout << "liveness analysis done" << std::endl;
        Graph<RegisterType> interference_graph = build_interference_graph(blocks);
        // std::cout << "interference graph done" << std::endl;
        Graph<RegisterType> interference_graph_copy = build_interference_graph(blocks);
        // std::cout << "interference graph copy done" << std::endl;
        auto [simplify_worklist, spill_worklist] = make_worklist(interference_graph);
        // std::cout << "worklist done" << std::endl;
    

        std::stack<GenericRegister> select_stack;
        do {
            if (!simplify_worklist.empty()) {
                simplify(interference_graph, spill_worklist, simplify_worklist, select_stack);
            } else if (!spill_worklist.empty()) {
                select_spill(interference_graph, spill_worklist, simplify_worklist);
            }
        } while (!simplify_worklist.empty() || !spill_worklist.empty());

        // std::cout << "simple and spill done" << std::endl;
        std::set<GenericRegister> colored_nodes;
        std::set<GenericRegister> spilled_nodes;
        std::map<GenericRegister, Register> color_mapping;
        assign_colors(interference_graph_copy, select_stack, spilled_nodes, colored_nodes, color_mapping);
        // std::cout << "assign colors done" << std::endl;
        if (!spilled_nodes.empty()) {
            // std::cout << "rewrite program" << std::endl;
            rewrite_program(*func, blocks, spill_worklist, colored_nodes, spilled_nodes, color_mapping);
            register_allocation_recursive(ir);
        } else {
            // std::cout << "apply color mapping" << std::endl;
            apply_color_mapping(*func, color_mapping);
        }
        for (Block *block : blocks) {
            delete block;
        }
        // std::cout << "done with function" << std::endl;
    }

}


void convert_arguments(IR &ir) {
    
    for (auto func : ir.functions) {
        std::map<GenericRegister, GenericRegister> argument_mapping;
        // Find argument mapping
        for (auto &instruction : func->code) {
            for (auto &arg : instruction.args) {
                if (std::holds_alternative<GenericRegister>(arg.target) && std::get<GenericRegister>(arg.target).local_id < 0) {
                    if (argument_mapping.find(std::get<GenericRegister>(arg.target)) == argument_mapping.end()) {
                        auto reg = func->new_register();
                        argument_mapping[std::get<GenericRegister>(arg.target)] = reg;
                    }
                }
            }
        }

        auto it = skip_activation_record_instructions(func->code);
        for (auto &mapping : argument_mapping) {
            long offset = mapping.first.local_id*(-8) + arg_offset;

            auto new_instruction = Instruction(Op::MOVQ, Arg(Register::RBP, IRL(offset)), Arg(mapping.second, DIR()), "move argument from stack");
            func->code.insert(it, new_instruction);
        }

        for (auto &instruction : func->code) {
            for (auto &arg : instruction.args) {
                if (std::holds_alternative<GenericRegister>(arg.target) && argument_mapping.find(std::get<GenericRegister>(arg.target)) != argument_mapping.end()) {
                    auto generic_arg = std::get<GenericRegister>(arg.target);
                    arg.target = argument_mapping[generic_arg];
                }
            }
        }
    }
}

void register_allocation(IR &ir) {
    // print ir
    // std::cout << ir << std::endl;
    convert_arguments(ir);
    register_allocation_recursive(ir);
}


