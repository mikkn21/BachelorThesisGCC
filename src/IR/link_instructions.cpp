#include "link_instructions.hpp"
#include <unordered_map>

bool is_jmp(Op op) {
    return op == Op::JMP || op == Op::JE || op == Op::JNE || op == Op::JL || op == Op::JLE || op == Op::JG || op == Op::JGE;
}

void link_instructions(IR& instructions) {
    std::unordered_map<std::string, Instruction*> map;
    for (auto& instruction : instructions) {
        if (instruction.operation == Op::LABEL) {
            if (std::holds_alternative<Label>(instruction.args[0].target))
                map[std::get<Label>(instruction.args[0].target).label] = &instruction;
        }
    }

    for (auto& instruction : instructions) {
        if (is_jmp(instruction.operation)) {
            if (std::holds_alternative<Label>(instruction.args[0].target)) {
                instruction.alternative_successor = map[std::get<Label>(instruction.args[0].target).label];
            }
        }
    }
}