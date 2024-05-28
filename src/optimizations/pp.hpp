#ifndef PEEP_HOLE
#define PEEP_HOLE

#include "liveness_analysis.hpp"
#include "../IR/ir.hpp"

struct WildCard{
    // empty
};

struct Pattern {
    std::list<std::variant<Op,WildCard>> components;
    std::list<std::function<bool(std::vector<Block*>, Pattern&)>> rules;
    std::list<Instruction> replacement;

    Pattern(std::list<std::variant<Op, WildCard>> components, std::list<std::function<bool(std::vector<Block*>, Pattern&)>> rules, std::list<Instruction> replacement) 
    : components(components), rules(rules), replacement(replacement) {}
};


void peephole_optimization(IR &ir); 


#endif