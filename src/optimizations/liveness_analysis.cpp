#include "liveness_analysis.hpp"
#include <iostream>
#include <algorithm>



bool RegisterTypeLess::operator()(const RegisterType& lhs, const RegisterType& rhs) const {
    return std::visit([&lhs, &rhs](const auto& v1, const auto& v2) {
        using T1 = std::decay_t<decltype(v1)>;
        using T2 = std::decay_t<decltype(v2)>;
        if constexpr (std::is_same_v<T1, T2>) {
            return v1 < v2;
        } else {
            return lhs.index() < rhs.index();
        }
    }, lhs, rhs);
}

Block::Block(
    std::list<Instruction> instructions,
    Live use,
    Live def,
    Live in,
    Live out,
    std::vector<Block*> successors
) : instructions(instructions), in(in), out(out), def(def), use(use), successors(successors) {}


std::ostream& operator<<(std::ostream& os, const RegisterType& registerType) {
    std::visit([&os](const auto& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, Register>) {
            os << static_cast<Register>(v);
        } else if constexpr (std::is_same_v<T, GenericRegister>) {
            os << "r" << v.local_id;
        }
    }, registerType);
    return os;
}



std::ostream& operator<<(std::ostream& os, const Block block) {
        os << "Block: \n";
        os << "In: ";
        for (auto reg : block.in) {
            os << reg << " ";
        }
        os << "\n";
        os << "Out: ";
        for (auto reg : block.out) {
            os << reg << " ";
        }
        os << "\n";
        os << "Def: ";
        for (auto reg : block.def) {
            os << reg << " ";
        }
        os << "\n";
        os << "Use: ";
        for (auto reg : block.use) {
            os << reg << " ";
        }
        os << "\n";
        os << "Successors: ";
        for (auto succ : block.successors) {
            os << succ << " ";
        }
        os << "\n";
        os << "Successors length: ";
        os << block.successors.size();
        os << "\n";

        return os;
}

std::ostream& operator<<(std::ostream& os, const LivenessAnalysis blocks) {
    for (auto block : blocks) {
        os << *block << std::endl;
    }
    return os;
}


bool is_defining_operation(Op op) {
    return op == Op::MOVQ || op == Op::ADDQ || op == Op::SUBQ || op == Op::IMULQ || op == Op::IDIVQ || op == Op::POPQ;
}

template<typename... Ts, typename Variant>
bool holds_any_of(const Variant& v) {
    return (std::holds_alternative<Ts>(v) || ...);
}

RegisterType get_register_type(TargetType reg) {
    if (std::holds_alternative<GenericRegister>(reg)) {
        return std::get<GenericRegister>(reg);
    } else if (std::holds_alternative<Register>(reg)) {
        return std::get<Register>(reg);
    } else {
        throw IRError("invalid cast from target type to register_type");
    }
}



void add_def_use_to_block(Block* current_block, Instruction instruction) {
    switch (instruction.operation) {
        case Op::MOVQ:
            if (holds_any_of<GenericRegister, Register>(instruction.args[0].target)) {
                current_block->use.insert(get_register_type(instruction.args[0].target));       
            }

            if (std::holds_alternative<IRL>(instruction.args[1].access_type) || std::holds_alternative<IND>(instruction.args[1].access_type)) {
                current_block->use.insert(get_register_type(instruction.args[1].target));
            }

            if (std::holds_alternative<DIR>(instruction.args[1].access_type)) { 
                current_block->def.insert(get_register_type(instruction.args[1].target));
            }
            break;
        case Op::POPQ:
            current_block->def.insert(get_register_type(instruction.args[0].target));
            break;
        case Op::ADDQ:
            if (holds_any_of<GenericRegister, Register>(instruction.args[0].target) && holds_any_of<GenericRegister, Register>(instruction.args[1].target)) {
                current_block->use.insert(get_register_type(instruction.args[0].target));
                current_block->use.insert(get_register_type(instruction.args[1].target));
                current_block->def.insert(get_register_type(instruction.args[1].target));
            } else if (holds_any_of<GenericRegister, Register>(instruction.args[0].target)) {
                current_block->use.insert(get_register_type(instruction.args[0].target));
                current_block->def.insert(get_register_type(instruction.args[0].target));
            } else if (holds_any_of<GenericRegister, Register>(instruction.args[1].target)) {
                current_block->use.insert(get_register_type(instruction.args[1].target));
                current_block->def.insert(get_register_type(instruction.args[1].target));
            } else {
                throw IRError("Invalid addq instruction arguments");
            }
            break;
        case Op::SUBQ:
            if (holds_any_of<GenericRegister, Register>(instruction.args[0].target) && holds_any_of<GenericRegister, Register>(instruction.args[1].target)) {
                current_block->use.insert(get_register_type(instruction.args[0].target));
                current_block->use.insert(get_register_type(instruction.args[1].target));
                current_block->def.insert(get_register_type(instruction.args[1].target));
            } else if (holds_any_of<GenericRegister, Register>(instruction.args[0].target)) {
                current_block->use.insert(get_register_type(instruction.args[0].target));
                current_block->def.insert(get_register_type(instruction.args[0].target));
            } else if (holds_any_of<GenericRegister, Register>(instruction.args[1].target)) {
                current_block->use.insert(get_register_type(instruction.args[1].target));
                current_block->def.insert(get_register_type(instruction.args[1].target));
            } else {
                throw IRError("Invalid subq instruction arguments");
            }
            break;
        case Op::IMULQ:
            if (instruction.args.size() == 1) {
                current_block->use.insert(Register::RAX); // result is saved to RDX:RAX
                if (holds_any_of<GenericRegister, Register>(instruction.args[0].target)) {
                    current_block->use.insert(get_register_type(instruction.args[0].target));
                }
                current_block->def.insert(Register::RDX); 
                current_block->def.insert(Register::RAX); 
            } else if (holds_any_of<GenericRegister, Register>(instruction.args[0].target) && holds_any_of<GenericRegister, Register>(instruction.args[1].target)) {
                current_block->use.insert(get_register_type(instruction.args[0].target));
                current_block->use.insert(get_register_type(instruction.args[1].target));
                current_block->def.insert(get_register_type(instruction.args[1].target));
            } else if (holds_any_of<GenericRegister, Register>(instruction.args[0].target)) {
                current_block->use.insert(get_register_type(instruction.args[0].target));
                current_block->def.insert(get_register_type(instruction.args[0].target));
            } else if (holds_any_of<GenericRegister, Register>(instruction.args[1].target)) {
                current_block->use.insert(get_register_type(instruction.args[1].target));
                current_block->def.insert(get_register_type(instruction.args[1].target));
            } else {
                throw IRError("Invalid imulq instruction arguments");
            }
            break;
        case Op::IDIVQ:
            current_block->use.insert(get_register_type(instruction.args[0].target));
            current_block->use.insert(Register::RAX);
            current_block->use.insert(Register::RDX);
            current_block->def.insert(Register::RAX); // Result is always written to RAX
            current_block->def.insert(Register::RDX); // Mod of result is always written to RDX
            break; 
        case Op::SYSCALL:
            current_block->use.insert(Register::RAX);
            current_block->def.insert(Register::RAX);
            current_block->use.insert(Register::RDI);
            current_block->use.insert(Register::RSI);
            current_block->use.insert(Register::RDX);
            current_block->use.insert(Register::R10);
            current_block->use.insert(Register::R8);
            current_block->use.insert(Register::R9);
            break;
        case Op::CALL:
            current_block->def.insert(Register::RAX);
            current_block->use.insert(Register::RDI);
            current_block->use.insert(Register::RBP);
            current_block->use.insert(Register::RSP);
            //current_block->def.insert(Register::RBP);
            break;
        case Op::RET:
            current_block->use.insert(Register::RAX);
            break;
        default:
            if (!instruction.args.empty() && holds_any_of<GenericRegister, Register>(instruction.args[0].target)) {
                current_block->use.insert(get_register_type(instruction.args[0].target));
            } 
            if (instruction.args.size() >= 2 && holds_any_of<GenericRegister, Register>(instruction.args[1].target)) {
                current_block->use.insert(get_register_type(instruction.args[1].target));
            } 
            break;
    }
}

std::string get_label_value(Instruction &instruction) {
    return std::get<Label>(instruction.args[0].target).label;
}

LivenessAnalysis construct_simple_blocks(const std::list<Instruction> &code) {
    std::unordered_map<std::string, Block*> labels_map;
    LivenessAnalysis blocks;
    size_t i = code.size();
    for (auto it = code.rbegin(); it != code.rend(); ++it) {
        --i;
        Instruction instruction = *it;
        Block *current_block = new Block();
        current_block->instructions = {instruction};

        if (instruction.operation == Op::LABEL) {
            labels_map[get_label_value(current_block->instructions.back())] = current_block;
        }

        if (i < code.size()-1 && instruction.operation != Op::JMP) { // JMP does not have the next instruction as successor
            current_block->successors.push_back(blocks.back());
        }

        add_def_use_to_block(current_block, instruction);
        blocks.push_back(current_block);
    }

    for (auto block : blocks) {
        for (auto instruction : block->instructions) {
            if (instruction.alternative_successor.has_value()) {
                std::string label = get_label_value(*instruction.alternative_successor.value());
                Block *label_block = labels_map[label];
                block->successors.push_back(label_block);
            }
        }
    }
    return blocks;
}


LivenessAnalysis liveness_analysis(const std::list<Instruction> &code) {

    LivenessAnalysis blocks = construct_simple_blocks(code);
    bool is_consistent;

    do {
        is_consistent = true;
        for (size_t i = 0; i < blocks.size(); i++) { 
            Live in_copy = blocks[i]->in;
            Live out_copy = blocks[i]->out;
            blocks[i]->in.clear();
            std::set_difference(blocks[i]->out.begin(), blocks[i]->out.end(),
                        blocks[i]->def.begin(), blocks[i]->def.end(),
                        std::inserter(blocks[i]->in, blocks[i]->in.begin()));
            blocks[i]->in.insert(blocks[i]->use.begin(), blocks[i]->use.end());
            blocks[i]->out.clear();
            for (auto *succ_block : blocks[i]->successors) {
                blocks[i]->out.insert(succ_block->in.begin(), succ_block->in.end());
            }
            is_consistent = (is_consistent && blocks[i]->in == in_copy && blocks[i]->out == out_copy);
        }
    } while (!is_consistent);
    std::reverse(blocks.begin(), blocks.end());
    return blocks;
}

