#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "parser/parser.hpp"

using namespace std;

int main(int argc, char* argv[]) {


    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1; // Exit with error code if no filename is provided
    }

    // Open the file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << "\n";
        return 1; // Exit with error code if the file cannot be opened
    }

    // Read the contents of the file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContent = buffer.str();

    // Close the file (optional here since it will be closed automatically when file goes out of scope)
    file.close();
    
    bool res = grammar::parser::parse(fileContent);

    if(res) {
        cout << "SUCESS" << endl; 
    } else {
        cout << "FAILURE" << endl; 
    }

    return 0;
}