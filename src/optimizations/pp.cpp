#include "pp.hpp"
#include "liveness_analysis.hpp"
#include <iostream>
#include <variant>


template<typename... Ts, typename Variant>
bool holds_any_of(const Variant& v) {
    return (std::holds_alternative<Ts>(v) || ...);
}

TargetType convert_register_type_to_target_type(RegisterType register_type) {
    if (std::holds_alternative<Register>(register_type)) {
        return std::get<Register>(register_type);
    } else if (std::holds_alternative<GenericRegister>(register_type)) {
        return std::get<GenericRegister>(register_type);
    }
    throw std::invalid_argument("Invalid register type");
}

// helper function to check if a target is an immediate value and has a specific value
bool has_immediate_value(TargetType target, int value) {
    return std::holds_alternative<ImmediateValue>(target) && std::get<ImmediateValue>(target).value == value;
}

/**
 * @brief List of patterns to be checked for peephole optimization.
 * 
 * @details Each lambda has to return a bool.
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
        WildCard()
    }, {
        [](std::vector<Block*> blocks, Pattern& pattern) {
            if (blocks.size() != 2) {
                throw std::invalid_argument("Invalid block size 2");
            }  


            Block b1 = *blocks.front();
            Block b2 = *blocks.back();

            auto &b1_args = b1.instructions.front().args;
            auto &b2_args = b2.instructions.front().args;
            if (b2_args.size() == 2) {
                if (((std::holds_alternative<DIR>(b1_args[0].access_type) || std::holds_alternative<DIR>(b2_args[1].access_type))
                    && std::holds_alternative<DIR>(b1_args[1].access_type))
                    && std::holds_alternative<DIR>(b2_args[0].access_type)
                    && std::holds_alternative<DIR>(b2_args[1].access_type)

                    && b2.use.find(*b1.def.begin()) != b2.use.end() 
                    && b2.def.find(*b1.def.begin()) == b2.def.end()
                    && b1.use.find(*b2.use.begin()) == b1.use.end() 
                    && !(std::holds_alternative<ImmediateValue>(b2_args[0].target) && std::holds_alternative<ImmediateValue>(b1_args[0].target))
                ) {
                    pattern.replacement.push_back(b1.instructions.front());
                    Instruction new_instruction = Instruction(b2.instructions.begin()->operation);

                    for (auto arg : b2_args) {
                        if (arg.target == b1_args[1].target) {
                            arg = b1_args[0];
                        }
                        new_instruction.args.push_back(arg);
                    }
                    pattern.replacement.push_back(new_instruction);


                    // std::cout << "\npattern:" << std::endl;
                    // std::cout << blocks[0]->instructions.front() << std::endl;
                    // std::cout << blocks[1]->instructions.front() << std::endl;
                    // std::cout << "replacement:" << std::endl;
                    // std::cout << *pattern.replacement.begin() << std::endl;
                    // std::cout << pattern.replacement.back() << std::endl;

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
                throw std::invalid_argument("Invalid block size 2");
            }  
            Block b1 = *blocks.front();
            Block b2 = *blocks.back();
            return b1.use == b2.def;
        }
    }, {/* replacement */}),
    // Pattern for redundant pop and push optimization
    Pattern ({
        Op::POPQ, 
        Op::PUSHQ   
    }, {
        [](std::vector<Block*> blocks, Pattern& pattern) {
            if (blocks.size() != 2) {
                throw std::invalid_argument("Invalid block size 2");
            }  
            Block b1 = *blocks.front();
            Block b2 = *blocks.back();

            return b1.def == b2.use && b1.out == b2.out;
        }
    }, {/* replacement */}),
    // Pattern to remove dead code
    Pattern ({
        WildCard()
    }, {
        [](std::vector<Block*> blocks, Pattern& pattern) {
            if (blocks.size() != 1) {
                std::cout << "size: " << blocks.size() << std::endl;
                throw std::invalid_argument("Invalid block size 1");
            }  
            Block b1 = *blocks.front();

            // if(b1.instructions.front().args.size() == 2) {
            //     if (std::holds_alternative<Register>(b1.instructions.front().args[0].target)) {
            //         if (std::get<Register>(b1.instructions.front().args[0].target) == Register::RSP) {
            //             return false;
            //         }
            //         if (std::get<Register>(b1.instructions.front().args[0].target) == Register::RBP) {
            //             return false;
            //         }
            //     }

            //     if (std::holds_alternative<Register>(b1.instructions.front().args[1].target)) {
            //         if (std::get<Register>(b1.instructions.front().args[1].target) == Register::RSP) {
            //             return false;
            //         }
            //         if (std::get<Register>(b1.instructions.front().args[1].target) == Register::RBP) {
            //             return false;
            //         }
            //     }
            // }

                        // if is defining operation and is not direct, then it is not dead code
            for (size_t i = 0; i < b1.instructions.front().args.size(); i++) {
                if (holds_any_of<GenericRegister, Register>(b1.instructions.front().args[i].target)) {
                    RegisterType target;
                    if (std::holds_alternative<GenericRegister>(b1.instructions.front().args[i].target)) {
                        target = std::get<GenericRegister>(b1.instructions.front().args[i].target);
                    } else {
                        target = std::get<Register>(b1.instructions.front().args[i].target);
                    }
                    if (b1.def.find(target) != b1.def.end() && holds_any_of<IRL, IND>(b1.instructions.front().args[i].access_type)) {
                        return false;
                    }
                }
            }

            if (b1.def.find(Register::RSP) != b1.def.end()) {
                return false;
            }




            // Push and Pop might be important for stack management so don't remove them
            if(b1.instructions.front().operation == Op::PUSHQ || b1.instructions.front().operation == Op::POPQ  ) {
                return false;
            }       

            if(b1.instructions.front().operation == Op::SYSCALL || b1.instructions.front().operation == Op::CALL) return false;
                if (b1.def.size() == 0) {
                    return false;
            }

            // define something but never use it = dead code
            if (b1.out.size() == 0 && b1.def.size() > 0) {
                return true; // replace with nothing
            }

            if (b1.out.find(*b1.def.begin()) == b1.out.end()) { //b1.def not in b1.out == dead code
                return true; // replace with nothing
            }
            return false;
        }
    }, {/* replacement */}),
    // pattern optimization for setting a register to 0 
    Pattern ({
        Op::MOVQ,
    }, {
        [](std::vector<Block*> blocks, Pattern& pattern) {
            if (blocks.size() != 1) {
                throw std::invalid_argument("Invalid block size 1");
            }  
            Block b1 = *blocks.front();
            auto &b1_args = b1.instructions.front().args;
            if (std::holds_alternative<DIR>(b1_args[1].access_type) && has_immediate_value(b1_args[0].target, 0)) {
                pattern.replacement.push_back(Instruction(Op::XORQ, b1_args[1], b1_args[1]));
                return true;
            }
            return false;
        }
    }, {/* replacement */}), 
    // // pattern jump to label under label optimization
    // Pattern ({
    //     Op::JMP,
    //     Op::LABEL
    // }, {
    //     [](std::vector<Block*> blocks, Pattern& pattern) {
    //         if (blocks.size() != 2) {
    //             throw std::invalid_argument("Invalid block size");
    //         }  
    //         Block b1 = *blocks.front();
    //         Block b2 = *blocks.back(); 

    //         if (std::get<Label>(b1.instructions.front().args[0].target).label == std::get<Label>(b2.instructions.front().args[0].target).label){
    //             pattern.replacement.push_back(b2.instructions.front());
    //             return true;
    //         }
    //         return false;
    //     }
    // }, {/* replacement */}),
    // pattern for moving from a register to itself
    Pattern ({
        Op::MOVQ,
    }, {
        [](std::vector<Block*> blocks, Pattern& pattern) {
            if (blocks.size() != 1) {
                throw std::invalid_argument("Invalid block size 1");
            }  
            Block b1 = *blocks.front();
            auto &b1_args = b1.instructions.front().args;
            //only if they both are DIR
            if (std::holds_alternative<DIR>(b1_args[0].access_type) && std::holds_alternative<DIR>(b1_args[1].access_type)) {
                //only if they are the same register
                if (std::holds_alternative<Register>(b1_args[0].target) && std::holds_alternative<Register>(b1_args[1].target)){
                    Register arg1 = std::get<Register>(b1_args[0].target);
                    Register arg2 = std::get<Register>(b1_args[1].target);
                    if (arg1 == arg2) {
                        return true;
                    }
                } else if (std::holds_alternative<GenericRegister>(b1_args[0].target) && std::holds_alternative<GenericRegister>(b1_args[1].target)){
                    GenericRegister arg1 = std::get<GenericRegister>(b1_args[0].target);
                    GenericRegister arg2 = std::get<GenericRegister>(b1_args[1].target);
                    if (arg1 == arg2) {
                        return true;
                    }
                }
            }   
            return false;
        }
    }, {/* replacement */}),

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
    // std::cout << "size: " << blocks.size() << std::endl;
    // std::cout << "made it into lamda" << std::endl;
    for(auto& rule : pattern.rules){
        // std::cout << "checking a rule" << std::endl;
        if (!rule(blocks, pattern)) {
            return false;
        }
    }
    return true;
}



void print_block(Block block) {
    if (block.instructions.front().args.size() > 0){
        if (std::holds_alternative<DIR>(block.instructions.front().args[0].access_type)) {
            std::cout << "Mem_access0: " << "DIR" << std::endl;
        } else if (std::holds_alternative<IRL>(block.instructions.front().args[0].access_type)) {
            std::cout << "Mem_access0: " << "IRL" << std::endl;
        } else if (std::holds_alternative<IND>(block.instructions.front().args[0].access_type)) {
            std::cout << "Mem_access0: " << "IND" << std::endl;
        }
        if (block.instructions.front().args.size() > 1 ) {
            if (std::holds_alternative<DIR>(block.instructions.front().args[1].access_type)) {
                std::cout << "Mem_access1: " << "DIR" << std::endl;
            } else if (std::holds_alternative<IRL>(block.instructions.front().args[1].access_type)) {
                std::cout << "Mem_access1: " << "IRL" << std::endl;
            } else if (std::holds_alternative<IND>(block.instructions.front().args[1].access_type)) {
                std::cout << "Mem_access1: " << "IND" << std::endl;
            }
        }
    }
    
    std::cout << "out: "; 
    for (auto out : block.out) {
        std::cout << out << " ";
    }
    std::cout << std::endl;
    std::cout << "def: ";
    for (auto def : block.def) {
        std::cout << def << " ";
    }
    std::cout << std::endl;
    std::cout << "use: ";
    for (auto use : block.use) {
        std::cout << use << " ";
    }
    std::cout << std::endl;
    
    std::cout << "in: ";
    for (auto in : block.in) {
        std::cout << in << " ";
    }
    std::cout << std::endl;
    
}

/**
 * Apply replacement to a portion of the IR based on the provided pattern starting from match_start to i.
 *
 * @param ir The intermediate representation to apply the replacement to
 * @param pattern The pattern containing the replacement information
 * @param match_start The starting index of the match in the intermediate representation
 * @param i The ending index of the match in the intermediate representation
 */
void apply_replacement(std::list<Instruction> &func, Pattern& pattern, size_t match_start, size_t i) {
    auto startIter = std::next(func.begin(), match_start);
    auto endIter = std::next(func.begin(), i + 1);

    // Calculate the number of elements to erase and the number of elements to insert
    int num_erased = std::distance(startIter, endIter);
    int num_inserted = pattern.replacement.size();

    // Erase the elements in the range
    auto end_point = func.erase(startIter, endIter);

    // Insert the replacement elements
    func.insert(end_point, pattern.replacement.begin(), pattern.replacement.end());

    // If the replacement is smaller, insert dummy elements to pad the list
    if (num_inserted < num_erased) {
        Instruction dummy = Instruction(Op::DUMMY); // Create a dummy instruction, define it appropriately
        func.insert(end_point, num_erased - num_inserted, dummy);
    }

}

void prune_dummy(std::list<Instruction> &func) {
    for(auto it = func.begin(); it != func.end(); it++) {
        if(it->operation == Op::DUMMY) {
            it = func.erase(it);
            it--;
        }
    }
}


void peephole_optimization2(IR &ir) {
    for(auto function : ir.functions) {
        bool cringe_bool = false; // indicates whether IR is stable
        while(!cringe_bool) {        
            cringe_bool = true;
            LivenessAnalysis blocks = liveness_analysis(function->code);
            bool changed = false;

            for (auto &pattern: patterns) {
                if (changed) {
                    for(auto block : blocks) delete block;
                    blocks = liveness_analysis(function->code);
                    changed = false;
                }


                size_t match_start = 0;
                auto pattern_op = pattern.components.begin();
                for (size_t i = 0; i < blocks.size(); i++) {
                    // std::cout << "-----------------------------------------" << std::endl;
                    // std::cout << blocks[i]->instructions.front() << std::endl;
                    // print_block(*blocks[i]);
                    if (std::holds_alternative<Op>(*pattern_op)) {
                        // std::cout << "in first if" << std::endl;
                        if (std::get<Op>(*pattern_op) != blocks[i]->instructions.front().operation) {
                            // std::cout << "in second if" << std::endl;
                            if (pattern_op != pattern.components.begin()) {
                                // std::cout << "in third if" << std::endl;
                                i--; // look at current block again with start of pattern
                            }
                            pattern_op = pattern.components.begin();
                            match_start = i+1;
                            continue; // pattern and block did not match look at next pattern
                        }
                    }
                    if (pattern_op == std::prev(pattern.components.end())) {
                        // std::cout << "Making subset replacement: " << match_start << " - " << i << std::endl;
                        if(check_lambda(std::vector<Block*>(blocks.begin() + match_start, blocks.begin() + i + 1), pattern)){
                            // std::cout << "test " << std::endl;
                            cringe_bool = false;
                            changed = true;
                            apply_replacement(function->code, pattern, match_start, i);
                            // std::cout << "-----------------------------------------" << std::endl;
                            // std::cout << "applying replacement" << std::endl;
                            // std:: cout << "replace: " << blocks[i]->instructions.front() << std::endl;
                            // if(!pattern.replacement.empty()) std::cout << "with: " << pattern.replacement.front() << std::endl;
                            pattern.replacement.clear(); // clear replacements to avoid adding duplicate code
                        }
                        pattern_op = pattern.components.begin();
                        match_start = i+1;
                        continue;
                    }
                    pattern_op++;
                } 
                prune_dummy(function->code); 
            }
            for(auto block : blocks) delete block;
        }
    }
}




void peephole_optimization(IR &ir) {
    for(auto function : ir.functions) {
        bool cringe_bool = false; // indicates whether IR is stable
        while(!cringe_bool) {        
            cringe_bool = true;

            LivenessAnalysis blocks = liveness_analysis(function->code);

            for (size_t i = 0; i < blocks.size(); ++i) {
                for (auto &pattern: patterns) {
                    size_t pattern_counter = 0;
                    bool changed = false;
                    for (auto &pattern_op : pattern.components) {
                        if (i+pattern_counter >= blocks.size()) {
                            break;
                        }
                        if (std::holds_alternative<Op>(pattern_op)) {
                            if (std::get<Op>(pattern_op) != blocks[i+pattern_counter]->instructions.front().operation) {
                                continue;
                            }
                        }
                        
                        if (pattern_counter+1 == pattern.components.size()) {
                            if (check_lambda(std::vector<Block*>(blocks.begin() + i, blocks.begin() + i + pattern_counter + 1), pattern)) {
                                cringe_bool = false;
                                apply_replacement(function->code, pattern, i, i+pattern_counter);
                                pattern.replacement.clear(); // clear replacements to avoid adding duplicate code
                                i = i + pattern_counter;
                                changed = true;
                            }
                        }
                        ++pattern_counter;
                    }
                    if (changed) {
                        break;
                    }
                }
                  
            } 
            prune_dummy(function->code); 
            for(auto block : blocks) delete block;

        }
    }
}

