#include "register_allocation.hpp"

Instruction mov_translate(Instruction instruction) {
    if (holds_alternative<ImmediateValue>(instruction.args[0].target)) {
        if (holds_alternative<GenericRegister>(instruction.args[1].target)) {
            //cout << get<GenericRegister>(instruction.args[1].target).id << endl;
            return Instruction(Op::MOVQ, instruction.args[0], Arg(Register::RBP, IRL((get<GenericRegister>(instruction.args[1].target).id)*(-8))), instruction.comment);
        } else if (holds_alternative<Register>(instruction.args[1].target)) {
            //pass
            throw IRError("Not implemented yet");
        } else {
            throw IRError("Invalid move instruction found");
        }
    } 
}

Instruction procedure_translate(Instruction instruction) {
    switch (get<Procedure>(instruction.args[0].target)) {
        case Procedure::PRINT:    
            if (holds_alternative<GenericRegister>(instruction.args[1].target)){
                return Instruction(Op::PROCEDURE, instruction.args[0], Arg(Register::RBP, IRL((get<GenericRegister>(instruction.args[1].target).id)*(-8))), instruction.comment);
            } else if (holds_alternative<ImmediateValue>(instruction.args[1].target)) {
                return instruction;
            } else {
                throw IRError("Invalid Print Instruction found");
            }
            break;
    }
}
// int x = 5
// print(x)
IR register_allocation(IR old_ir) {
    IR ir;
    for (auto &instruction : old_ir) {
        switch (instruction.operation)
        {
        case Op::MOVQ:
            ir.push_back(mov_translate(instruction));
            break;
        case Op::PROCEDURE:
            ir.push_back(procedure_translate(instruction));
            break;
        default:
            break;
        }
    }

    return ir; // is not copied. The compiler will optimize it by using copy elision
}