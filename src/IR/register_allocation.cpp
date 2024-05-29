#include "register_allocation.hpp"
#include <set>
#include <map>

const int callee_offset = -48;
const int arg_offset = 16;

std::list<Instruction> generic_translate(Instruction instruction, std::map<GenericRegister, long> stack_offset_map) { // Does not work with procedures.
    std::list<Instruction> instructions;
    Instruction translated_instruction = Instruction(instruction.operation, instruction.comment);
    std::vector<Register> registers = {Register::R11, Register::R12, Register::R13};
    // Translates generic registers to concrete registers.
    for (size_t i = 0; i < instruction.args.size(); i++) {
        auto arg = instruction.args[i];
        if (std::holds_alternative<GenericRegister>(arg.target)) {
            long offset = stack_offset_map[std::get<GenericRegister>(arg.target)];
            instructions.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, IRL(offset)), Arg(registers[i], DIR()), "Generic Register Translation"));
            translated_instruction.args.push_back(Arg(registers[i], arg.access_type));
        } else {
            translated_instruction.args.push_back(arg);
        }
    }
    instructions.push_back(translated_instruction);

    for (size_t i = 0; i < instruction.args.size(); i++) {
        auto arg = instruction.args[i];
        if (std::holds_alternative<GenericRegister>(arg.target)) {
            long offset = stack_offset_map[std::get<GenericRegister>(arg.target)];
            instructions.push_back(Instruction(Op::MOVQ, Arg(registers[i], DIR()), Arg(Register::RBP, IRL(offset)), "move result back to Generic Register"));
        } 
    }
    return instructions;
}

/// @brief Helper function for procedure_translate for instructions with one argument
Instruction procedureTranslateOneArgument(Instruction instruction, std::string error) {
    // TODO: Might get an error because it is a Register and not a GenericRegister,  but we don't know why it works for print
    if (std::holds_alternative<GenericRegister>(instruction.args[1].target)){
        auto id = std::get<GenericRegister>(instruction.args[1].target).local_id;
        if (id > 0) {
            return Instruction(Op::PROCEDURE, instruction.args[0], Arg(Register::RBP, IRL((std::get<GenericRegister>(instruction.args[1].target).local_id)*(-8)+callee_offset)), instruction.comment);
        } else if (id < 0) {
            return Instruction(Op::PROCEDURE, instruction.args[0], Arg(Register::RBP, IRL((std::get<GenericRegister>(instruction.args[1].target).local_id)*(-8)+arg_offset)), instruction.comment);
        } else {
            throw IRError("Invalid Generic Register found");
        }
    } else if (std::holds_alternative<ImmediateValue>(instruction.args[1].target)) {
        return instruction;
    } else if (std::holds_alternative<Register>(instruction.args[1].target)) {
        return instruction;
    } else {
        throw IRError(error);
    }
}


Instruction procedure_translate(Instruction instruction) {
    switch (std::get<Procedure>(instruction.args[0].target)) {
        case Procedure::PRINT:    
            return procedureTranslateOneArgument(instruction, "Invalid Print Instruction found");
            break;
        case Procedure::MEM_ALLOC:
            return procedureTranslateOneArgument(instruction, "Invalid Mem Alloc Instruction found");
            break;
        default:
            return instruction;
    }
}

bool is_end_of_callee_save(Instruction &instruction) {
    return instruction.comment.value_or("") == "END OF CALLEE SAVE";
}

void naive_register_allocation(IR &ir) {
    for (auto func : ir.functions) {
        std::map<GenericRegister, long> stack_offset_map;
        std::list<Instruction> code;
        for (auto &instruction : (*func).code) {
            for (auto &arg : instruction.args) {
                if (std::holds_alternative<GenericRegister>(arg.target)) {
                    auto arg_target = std::get<GenericRegister>(arg.target);
                    if (stack_offset_map.find(arg_target) == stack_offset_map.end()) {
                        if (arg_target.local_id < 0) {
                            stack_offset_map[arg_target] = arg_target.local_id * (-8) + arg_offset;
                        } else {
                            stack_offset_map[arg_target] = func->new_stack_slot() * (-8) + callee_offset;
                        }
                    }
                }
            }
        }

        for (auto &instruction : (*func).code) {
            switch (instruction.operation)
            {
                // case Op::MOVQ:
                //     ir.push_back(generic_translate(instruction));
                //     break;
                case Op::PROCEDURE:
                    code.push_back(procedure_translate(instruction));
                    break;
                default:
                    std::list<Instruction> translated_instructions = generic_translate(instruction, stack_offset_map);
                    code.insert(code.end(), translated_instructions.begin(), translated_instructions.end());
                    if (is_end_of_callee_save(instruction)) {
                        for (size_t i = 0; i < stack_offset_map.size(); i++) {
                            code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()), "Setting temporary variable to 0"));
                        }
                    }
                    break;
            }
        }
        func->code = code;
    }
}