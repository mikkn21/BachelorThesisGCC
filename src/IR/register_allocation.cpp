#include "register_allocation.hpp"

const int callee_offset = -40;
const int arg_offset = 16;


Instruction generic_translate(Instruction instruction) {
    for (auto &arg : instruction.args) {
        if (std::holds_alternative<GenericRegister>(arg.target)) {
            auto id = std::get<GenericRegister>(arg.target).local_id;
            if (id > 0) {
                arg = Arg(Register::RBP, IRL((std::get<GenericRegister>(arg.target).local_id)*(-8)+callee_offset));
            } else if (id < 0) {
                arg = Arg(Register::RBP, IRL((std::get<GenericRegister>(arg.target).local_id)*(-8)+arg_offset));
            } else {
                throw IRError("Invalid Generic Register found");
            }
        }
    }
    return instruction;
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
    } else {
        throw IRError(error);
    }
}

/// @brief translate a generic instruction to a IRL instruction
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
            ir.push_back(generic_translate(instruction));
            break;
        }
    }

    return ir; // is not copied. The compiler will optimize it by using copy elision
}