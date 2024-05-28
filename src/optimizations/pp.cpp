#include "pp.hpp"


std::list<Pattern> patterns;

/**
 * Perform a series of checks on a list of blocks using a list of lambda functions.
 *
 * @param blocks A vector of Block pointers to be checked
 * @param rules A list of lambda functions that take a vector of Block pointers and return a boolean
 *
 * @return true if all lambda functions return true, false otherwise
 */
bool check_lambda(std::vector<Block*> blocks, std::list<std::function<bool(std::vector<Block*>)>> rules) {
    for(auto& rule : rules){
        if (!rule(blocks)) {
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
void apply_replacement(IR &ir, Pattern pattern, int match_start, int i) {
    auto startIter = std::next(ir.begin(), match_start);
    auto endIter = std::next(ir.begin(), i + 1);
    auto end_point = ir.erase(startIter, endIter);
    ir.insert(end_point, pattern.replacement.begin(), pattern.replacement.end());
}


void peephole_optimization(IR &ir) {
    
    LivenessAnalysis blocks = liveness_analysis(ir);

    Pattern test({
        Op::PUSHQ, 
        Op::POPQ
    }, {
        [](std::vector<Block*> blocks) {
            if (blocks.size() != 2) {
                throw std::invalid_argument("Invalid block size");
            }  
            Block b1 = *blocks.front();
            Block b2 = *blocks.back();
            return b1.use == b2.def;
        }
    }, {/* replacement */});

    patterns.push_back(test);

    // Put this into another loop to repeat until pp is stabile
    bool cringe_bool = false;
    while(!cringe_bool) {
        std::cout << "Cringing" << std::endl;
        cringe_bool = true;
        for (auto pattern: patterns) {
            int match_start = 0;
            auto pattern_op = pattern.components.begin();
            for (int i = 0; i < blocks.size(); i++) {
                std::cout << "pattern op: " << *pattern_op << std::endl;
                std::cout << "block op: " << (*blocks[i]).instructions.front().operation << std::endl;
                if ((*blocks[i]).instructions.front().operation != *pattern_op) {
                    std::cout << "not match" << std::endl;
                    pattern_op = pattern.components.begin();
                    match_start = i+1;
                    continue; // pattern and block did not match look at next pattern
                }
                if (pattern_op == std::prev(pattern.components.end())) {
                    std::cout << "Found match" << std::endl;
                    if(check_lambda(std::vector<Block*>(blocks.begin() + match_start, blocks.begin() + i + 1), pattern.rules)){
                        cringe_bool = false;
                        apply_replacement(ir, pattern, match_start, i);
                    }
                }
                pattern_op++;
            }   
        }
    }
    
    for(auto block : blocks) delete block;
}

