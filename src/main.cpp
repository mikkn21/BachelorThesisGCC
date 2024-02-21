#include <cstdio>
#include <iostream>
#include <ostream>
#include <cstring>


#include "compiler.hpp"

using namespace std;
using namespace grammar::compiler;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "You dumb fuck\n";
        return 1; 
    }

    // The file name has to be the last arg!! 
    std::string_view fileName = argv[argc - 1];
    CompilerOptions *options = new CompilerOptions();

    // Look for flags and options:
    for(int i = 1; i < argc - 1; i++) {  // loop after your own filename (i.e., prog.out) up until fileName 
        if (strcmp(argv[i], "-P") == 0) { // Stop after parsing
            options->stopAfter = StopAfterParser; 
            printf("saw -P\n");
        } else if (strcmp(argv[i], "-p") == 0) { // print AST
            options->printAst = true; 
            printf("saw -p\n");
        } else { // default
            std::cerr << "Error on loading option: " << argv[i] << endl;
            return -1;
        }   
    }
    
    grammar::compiler::compile(fileName, *options);

    
    return 0;
}
