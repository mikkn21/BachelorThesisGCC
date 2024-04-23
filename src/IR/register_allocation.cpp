#include "register_allocation.hpp"

const int callee_offset = -40;
const int arg_offset = 16;

std::vector<Instruction> generic_translate(Instruction instruction) {
    std::vector<Instruction> instructions;
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

Instruction procedure_translate(Instruction instruction) {
    switch (std::get<Procedure>(instruction.args[0].target)) {
        case Procedure::PRINT:    
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
            } else {
                throw IRError("Invalid Print Instruction found");
            }
            break;
        default:
            return instruction;
    }
}



IR register_allocation(IR old_ir) {
    IR ir;
    for (auto &instruction : old_ir) {
        switch (instruction.operation)
        {
        // case Op::MOVQ:
        //     ir.push_back(generic_translate(instruction));
        //     break;
        case Op::PROCEDURE:
            ir.push_back(procedure_translate(instruction));
            break;
        default:
            {
                std::vector<Instruction> translated_instructions = generic_translate(instruction);
                ir.insert(ir.end(), translated_instructions.begin(), translated_instructions.end());
            }
            break;
        }
    }

    return ir; // is not copied. The compiler will optimize it by using copy elision
}