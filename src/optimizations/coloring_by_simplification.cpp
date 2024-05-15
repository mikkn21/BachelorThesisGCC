#include "../IR/ir.hpp"
#include "control_flow_graph.hpp"
#include "liveness_analysis.hpp"


IR register_allocation2(IR old_ir) {
    LivenessAnalysis blocks = liveness_analysis(old_ir);
    // std::cout << blocks << std::endl;

    for (Block* block : blocks) {
        delete block;
    }
    
    return old_ir;
}

/* 
procedure Main()
    LivenessAnalysis()
    Build()
    MakeWorklist()
    repeat
        if simplifyWorklist != {} then Simplify()
        else if worklistMoves != {} then Coalesce()
        else if freezeWorklist != {} then Freeze()
        else if spillWorklist != {} then SelectSpill()
    until simplifyWorklist = {} ∧ worklistMoves = {} ∧ freezeWorklist = {} ∧ spillWorklist = {}
    AssignColors()
    if spilledNodes != {} then
        RewriteProgram(spilledNodes)
        Main() 
        
*/