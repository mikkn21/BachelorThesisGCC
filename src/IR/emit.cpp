#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "emit.hpp"

void emit_to_file(IR ir) {
    std::ofstream outputFile("chad.s");
    outputFile << ".globl _start" << endl;
    outputFile << "\n_start:" << endl;
    if (outputFile.is_open()) {
        for (const Instruction& instruction : ir) {
            // MAKE SWITCH STATEMENT TO CHECK FOR PROCEDURES MAYBE?
            outputFile << "\t" << instruction << std::endl;
        }
        outputFile.close();
    } else {
        throw EmitError("Could not open/create output file");
    }
}

void emit(IR ir) {
    emit_to_file(move(ir));
}