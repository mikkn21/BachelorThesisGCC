#include "register_allocation.hpp"

Instruction mov_translate(Instruction instruction) {
    if (holds_alternative<ImmediateValue>(instruction.args[0].target)) {
        if (holds_alternative<GenericRegister>(instruction.args[1].target)) {
            return Instruction(Op::MOVQ, instruction.args[0], Arg(Register::RBP, IRL((get<GenericRegister>(instruction.args[1].target).id+1)*(-8))), instruction.comment);
        }// } else if (holds_alternative<Register>(instruction.args[1].target)) {

        // }
    } 
}

IR register_allocation(IR old_ir) {
    IR ir;
    for (auto &instruction : old_ir) {
        switch (instruction.operation)
        {
        case Op::MOVQ:
            ir.push_back(mov_translate(instruction));
            break;
        
        default:
            break;
        }
    }

    return ir; // is not copied. The compiler will optimize it by copying elision
}