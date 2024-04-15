#include <fstream>
#include "compiler.hpp"
#include "parser/parser.hpp"
#include "semantics/symbol_collection_phase2.hpp"
#include "type_checking/typeChecking.hpp"
#include "semantics/symbol_collection.hpp"


namespace grammar::compiler {
    std::string getFileContent(std::string_view input) {
        if(input.empty()) {
            std::cerr << "Error: filename is empty!" << std::endl;
            throw std::invalid_argument("Filename is empty"); 
        }

        std::ifstream file(input.data());
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << input << std::endl;
            throw std::invalid_argument("Could not open file correctly, ABORT! "); 
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();
        return content;
    }

    // This is compiling from a filename
    std::unique_ptr<CompilerReturnObj> compileFromFile(std::string_view input, const CompilerOptions &options) {
        std::string content = getFileContent(input);
        auto obj = compileFromString(std::string_view(content), options);

        return obj;
    }

    // compile from a string
    std::unique_ptr<CompilerReturnObj> compileFromString(std::string_view input, const CompilerOptions &options) {
        if (options.printInput) {
            std::cout << "Input:\n" << input << endl << endl; // extra endl, so the read file is printed pretty
        }

        auto obj = std::make_unique<CompilerReturnObj>();
        obj->ast = parser::parse(input);
        
        // print ast tree if option is enabled
        if (options.printAst) {
            std::cout << "AST:\n" << obj->ast << endl;
        }

        if (options.stopAfter == StopAfterParser ) {
            return obj;
        }

        std::unique_ptr<SymbolTable> globalScope = std::make_unique<SymbolTable>();
        symbol_collection(obj->ast, globalScope.get());
        symbol_collection_phase2(obj->ast, globalScope.get());

        if (options.stopAfter == StopAfterSymbolCollection ) {
            return obj;
        }

        obj->ast = typeChecker(obj->ast, globalScope.get());

        if (options.stopAfter == StopAfterTypeCheck ) {
            return obj;
        }
        obj->ir = intermediate_code_generation(obj->ast); 

        if (options.printCodeGeneration){
            std::cout << "CodeGen:\n" << obj->ir << std::endl;
        }
        if (options.stopAfter == StopAfterCodeGen){
            return obj;
        }
        obj->ir = register_allocation(obj->ir); 

        if (options.stopAfter == StopAfterRegAlloc){
            return obj;
        }

        if (options.printRegisterAllocation){
            std::cout << "RegisterAllocation:\n" << obj->ir << std::endl;
        }

        emit(obj->ir);        

        obj->setGlobalScope(std::move(globalScope));

        return obj;
    }


} // namespace grammar::compiler
