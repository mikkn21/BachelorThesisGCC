#ifndef LIVENESS_ANALYSIS
#define LIVENESS_ANALYSIS

#include <vector>
#include <set>
#include <ostream>
#include "../IR/ir.hpp"

using RegisterType = std::variant<Register, GenericRegister>;

struct RegisterTypeLess {
    bool operator()(const RegisterType& lhs, const RegisterType& rhs) const;
};

using Live = std::set<RegisterType, RegisterTypeLess>;

struct Block {
    IR instructions;
    Live in;
    Live out;
    Live def;
    Live use;
    std::vector<Block*> successors;

    Block(
        IR instructions = {},
        Live use = {},
        Live def = {},
        Live in = {},
        Live out = {},
        std::vector<Block*> successors = {}
    );
};

using LivenessAnalysis = std::vector<Block*>;

LivenessAnalysis liveness_analysis(IR &code);

std::ostream& operator<<(std::ostream& os, const RegisterType& registerType);
std::ostream& operator<<(std::ostream& os, const Block block);
std::ostream& operator<<(std::ostream& os, const LivenessAnalysis blocks);


#endif // LIVENESS_ANAYLSIS