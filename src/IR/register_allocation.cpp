#include "register_allocation.hpp"
#include <set>

const int callee_offset = -40;
const int arg_offset = 16;

std::list<Instruction> generic_translate(Instruction instruction) { // Does not work with procedures.
    std::list<Instruction> instructions;
    Instruction translated_instruction = Instruction(instruction.operation, instruction.comment);
    std::vector<Register> registers = {Register::R11, Register::R12, Register::R13};
    // Translates generic registers to concrete registers.
    for (size_t i = 0; i < instruction.args.size(); i++) {
        auto arg = instruction.args[i];
        if (std::holds_alternative<GenericRegister>(arg.target)) {
            auto id = std::get<GenericRegister>(arg.target).local_id;
            if (id == 0) {
                throw IRError("Invalid Generic Register found");
            }
            long offset = std::get<GenericRegister>(arg.target).local_id*(-8) + ( id > 0 ? callee_offset : arg_offset);
            instructions.push_back(Instruction(Op::MOVQ, Arg(Register::RBP, IRL(offset)), Arg(registers[i], DIR()), "Generic Register Translation"));
            translated_instruction.args.push_back(Arg(registers[i], arg.access_type));
        } else {
            translated_instruction.args.push_back(arg);
        }
    }
    instructions.push_back(translated_instruction);

    for (size_t i = instruction.args.size(); i > 0; i--) {
        auto j = i-1;
        auto arg = instruction.args[j];
        if (std::holds_alternative<GenericRegister>(arg.target)) {
            auto id = std::get<GenericRegister>(arg.target).local_id;
            long offset = std::get<GenericRegister>(arg.target).local_id*(-8) + ( id > 0 ? callee_offset : arg_offset);
            instructions.push_back(Instruction(Op::MOVQ, Arg(registers[j], DIR()), Arg(Register::RBP, IRL(offset)), "move result back to Generic Register"));
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


void naive_register_allocation(IR &ir) {
    for (auto func : ir.functions) {
        std::list<Instruction> code;
        std::set<GenericRegister> generic_registers;
        for (auto &instruction : (*func).code) {
            for (auto &arg : instruction.args) {
                if (std::holds_alternative<GenericRegister>(arg.target)) {
                    generic_registers.insert(std::get<GenericRegister>(arg.target));
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
                    if (std::get<Procedure>(instruction.args[0].target) == Procedure::CALLEE_SAVE) {
                        for (size_t i = 0; i < generic_registers.size(); i++) {
                            code.push_back(Instruction(Op::PUSHQ, Arg(ImmediateValue(0), DIR()), "Setting temporary variable to 0"));
                        }
                    }
                    break;
                default:
                    std::list<Instruction> translated_instructions = generic_translate(instruction);
                    code.insert(code.end(), translated_instructions.begin(), translated_instructions.end());
                    break;
            }
        }
        func->code = code;
    }
}
