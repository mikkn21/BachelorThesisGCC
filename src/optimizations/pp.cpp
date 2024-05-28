#include "pp.hpp"
#include "liveness_analysis.hpp"
#include <iostream>
#include <variant>


// helper function to check if a target is an immediate value and has a specific value
bool has_immediate_value(TargetType target, int value) {
    return std::holds_alternative<ImmediateValue>(target) && std::get<ImmediateValue>(target).value == value;
}

/**
 * @brief List of patterns to be checked for peephole optimization.
 * 
 * @de½ails Each lambda has to return a bool.
 * The `replacement` is empty until a lambda function inserts a replacement instruction.
 * 
 * @warning The order of the patterns is important, as the first pattern that matches will be applied.
 * 
 * Add more patterns here.
 */
std::list<Pattern> patterns = {
    // Pattern for A->B, B->C => A->C(transitive move) optimization
    Pattern({
        Op::MOVQ,
        Op::MOVQ
    }, {
        [](std::vector<Block*> blocks, Pattern& pattern) {
            if (blocks.size() != 2) {
                throw std::invalid_argument("Invalid block size");
            }  
            Block b1 = *blocks.front();
            Block b2 = *blocks.back();
            if (b1.def == b2.use) {
                auto &b1_args = b1.instructions.front().args;
                auto &b2_args = b2.instructions.front().args;
                
                // Only use this optimization if all the registers are accessed directly
                if (std::holds_alternative<DIR>(b1_args[0].access_type) && std::holds_alternative<DIR>(b2_args[0].access_type) 
                    && std::holds_alternative<DIR>(b1_args[1].access_type) && std::holds_alternative<DIR>(b2_args[1].access_type)
                ){
                    pattern.replacement.push_back(Instruction(Op::MOVQ, b1.instructions.front().args[0], b2.instructions.front().args[1]));
                    return true;
                }   
            }
            return false;
        }
    }, {/* replacement */}),
    // Pattern for redundant push and pop optimization
    Pattern ({
        Op::PUSHQ, 
        Op::POPQ
    }, {
        [](std::vector<Block*> blocks, Pattern& pattern) {
            if (blocks.size() != 2) {
                throw std::invalid_argument("Invalid block size");
            }  
            Block b1 = *blocks.front();
            Block b2 = *blocks.back();
            return b1.use == b2.def;
        }
    }, {/* replacement */}),
    // Pattern to remove dead code
    // Pattern ({
    //     WildCard()
    // }, {
    //     [](std::vector<Block*> blocks, Pattern& pattern) {
    //         if (blocks.size() != 1) {
    //             throw std::invalid_argument("Invalid block size");
    //         }  
    //         Block b1 = *blocks.front();
    //         if (b1.out.find(*b1.def.begin()) != b1.out.end()) { //b1.def not in b1.out == dead code
    //             return true; // replace with nothing
    //         }
    //         return false;

    //     }
    // }, {/* replacement */}),

    // pattern optimization for setting a register to 0 
    Pattern ({
        Op::MOVQ,
    }, {
        [](std::vector<Block*> blocks, Pattern& pattern) {
            if (blocks.size() != 1) {
                throw std::invalid_argument("Invalid block size");
            }  
            Block b1 = *blocks.front();
            auto &b1_args = b1.instructions.front().args;
            
            if (std::holds_alternative<DIR>(b1_args[0].access_type) && has_immediate_value(b1_args[0].target, 0)) {
                pattern.replacement.push_back(Instruction(Op::XORQ, b1_args[1], b1_args[1]));
                return true;
            }
            return false;
        }
    }, {/* replacement */})
};

/**
 * Perform a series of checks on a list of blocks using a list of lambda functions.
 *
 * @param blocks A vector of Block pointers to be checked
 * @param rules A list of lambda functions that take a vector of Block pointers and return a boolean
 *
 * @return true if all lambda functions return true, false otherwise
 */
bool check_lambda(std::vector<Block*> blocks, Pattern &pattern) {
    for(auto& rule : pattern.rules){
        if (!rule(blocks, pattern)) {
            return false;
        }
    }
    return true;
}

/**
 * Apply replacement to a portion of the IR based on the provided pattern starting from match_start to i.
 *
 * @param ir The intermediate representation to apply the replacement to
 * @param pattern The pattern containing the replacement information
 * @param match_start The starting index of the match in the intermediate representation
 * @param i The ending index of the match in the intermediate representation
 */
void apply_replacement(IR &ir, Pattern& pattern, int match_start, int i) {
    auto startIter = std::next(ir.begin(), match_start);
    auto endIter = std::next(ir.begin(), i + 1);
    auto end_point = ir.erase(startIter, endIter);
    ir.insert(end_point, pattern.replacement.begin(), pattern.replacement.end());
}


void peephole_optimization(IR &ir) {
    std::cout << "Just entered peephole_optimization" << std::endl;
    bool cringe_bool = false; // indicates whether IR is stable
    while(!cringe_bool) {
        std::cout << "whileloop\n";
        
        cringe_bool = true;
        for (auto pattern: patterns) {
            std::cout << "foreach loop" << std::endl;
            LivenessAnalysis blocks = liveness_analysis(ir);
            std::cout << "HAHAHAHHAHAHA" << std::endl;
            if(blocks.empty()) std::cout << "not blocks" << std::endl;
            int match_start = 0;
            auto pattern_op = pattern.components.begin();
            
            for (size_t i = 0; i < blocks.size(); i++) {
                std::cout << "for loop" << std::endl;
                if (std::holds_alternative<Op>(*pattern_op)) {
                    std::cout << "holds op" << std::endl;
                    if (std::get<Op>(*pattern_op) != blocks[i]->instructions.front().operation) {
                        std::cout << "not a match" << std::endl;
                        if (pattern_op != pattern.components.begin()) {
                            i--; // look at current block again with start of pattern
                        }
                        pattern_op = pattern.components.begin();
                        match_start = i+1;
                        continue; // pattern and block did not match look at next pattern
                        }
                    } 
                if (pattern_op == std::prev(pattern.components.end())) {
                    if(check_lambda(std::vector<Block*>(blocks.begin() + match_start, blocks.begin() + i + 1), pattern)){
                        cringe_bool = false;
                        apply_replacement(ir, pattern, match_start, i);
                        pattern_op = pattern.components.begin();
                        continue;
                    }
                }
                pattern_op++;
            }   
            for(auto block : blocks) delete block;
        }
        
    }
}

