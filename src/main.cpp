#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
 
int main(int argc, char* argv[]) {4

    if (argc < 2) {
        std::cerr << "You dumb fuck\n";
        return 1; 
    }

    // The file name has to be the last arg
    std::string_view fileName = argv[argc - 1];

    // loop after your own filename (i.e., prog.out) up until fileName 
    for(int i = 1; i < argc - 2; i++) {
        printf("%s", argv[i]);
    }

    // Open the file
    // std::ifstream file(argv[1]);
    // if (!file.is_open()) {
    //     std::cerr << "Error opening file: " << argv[1] << "\n";
    //     return 1; 
    // }

    // std::stringstream buffer;
    // buffer << file.rdbuf();
    // std::string fileContent = buffer.str();

    // file.close();
    
    return 0;
}