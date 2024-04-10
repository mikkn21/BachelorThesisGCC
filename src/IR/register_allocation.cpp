#include "register_allocation.hpp"

Instruction generic_translate(Instruction instruction) {
    for (auto &arg : instruction.args) {
        if (holds_alternative<GenericRegister>(arg.target)) {
            arg = Arg(Register::RBP, IRL((get<GenericRegister>(arg.target).local_id)*(-8)));
        }
    }
    return instruction;
}

Instruction procedure_translate(Instruction instruction) {
    switch (get<Procedure>(instruction.args[0].target)) {
        case Procedure::PRINT:    
            if (holds_alternative<GenericRegister>(instruction.args[1].target)){
                return Instruction(Op::PROCEDURE, instruction.args[0], Arg(Register::RBP, IRL((get<GenericRegister>(instruction.args[1].target).local_id)*(-8))), instruction.comment);
            } else if (holds_alternative<ImmediateValue>(instruction.args[1].target)) {
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
            ir.push_back(generic_translate(instruction));
            break;
        }
    }

    return ir; // is not copied. The compiler will optimize it by using copy elision
}