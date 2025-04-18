#include <fstream>
#include "compiler.hpp"
#include "IR/ir.hpp"
#include "parser/parser.hpp"
#include "semantics/symbol_collection_phase2.hpp"
#include "type_checking/typeChecking.hpp"
#include "semantics/symbol_collection.hpp"
#include "optimizations/coloring_by_simplification.hpp"
#include "optimizations/pp.hpp"


namespace grammar::compiler {
    std::string get_file_content(std::string_view input) {
        if (input.empty()) {
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
    std::unique_ptr<CompilerReturnObj> compile_from_file(std::string_view input, const CompilerOptions &options) {
        std::string content = get_file_content(input);
        auto obj = compile_from_string(std::string_view(content), options);

        return obj;
    }

    // compile from a string
    std::unique_ptr<CompilerReturnObj> compile_from_string(std::string_view input, const CompilerOptions &options) {
        if (options.print_input) {
            std::cout << "Input:\n" << input << endl << endl; // extra endl, so the read file is printed pretty
        }

        auto obj = std::make_unique<CompilerReturnObj>();
        obj->ast = parser::parse(input);
        
        // print ast tree if option is enabled
        if (options.print_ast) {
            std::cout << "AST:\n" << obj->ast << endl;
        }

        if (options.stop_after == StopAfterParser ) {
            return obj;
        }

        obj->global_scope = std::make_unique<SymbolTable>();
        obj->ast.global_scope = obj->global_scope.get();

        symbol_collection(obj->ast);
        symbol_collection_phase2(obj->ast);

        if (options.stop_after == StopAfterSymbolCollection ) {
            return obj;
        }

        type_checker(obj->ast);

        if (options.stop_after == StopAfterTypeCheck ) {
            return obj;
        }
        obj->ir = intermediate_code_generation(obj->ast); 
        if (options.print_code_generation) {
            std::cout << "CodeGen:\n" << *obj->ir << std::endl;
        }


        if (options.stop_after == StopAfterCodeGen) {
            return obj;
        }
      
        if (options.use_peephole) {
            peephole_optimization(*obj->ir);
        } 

        if (options.stop_after == stopAfterPeepHole){
            return obj;
        }
        if (options.naive_register_allocation){
            naive_register_allocation(*obj->ir); 
        } else {
            register_allocation(*obj->ir); 
        }


        if (options.stop_after == StopAfterRegAlloc){
            return obj;
        }

        
        if (options.print_register_allocation){
            std::cout << "RegisterAllocation:\n" << obj->ir << std::endl;
        }

        if (options.use_peephole) {
            peephole_optimization(*obj->ir);
        } 

        emit(*obj->ir, options.output_file);        

        return obj;
    }
    
    CompilerReturnObj::~CompilerReturnObj() {
        delete ir;
    }
} // namespace grammar::compiler
