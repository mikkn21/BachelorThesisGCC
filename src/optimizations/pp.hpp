#ifndef PEEP_HOLE
#define PEEP_HOLE

#include "liveness_analysis.hpp"
#include "../IR/ir.hpp"


struct Pattern {
    std::list<Op> components;
    std::list<std::function<bool(std::vector<Block*>)>> rules;
    std::list<Instruction> replacement;

    Pattern(std::list<Op> components, std::list<std::function<bool(std::vector<Block*>)>> rules, std::list<Instruction> replacement) 
    : components(components), rules(rules), replacement(replacement) {}
};

void peephole_optimization(IR &ir); 


#endif