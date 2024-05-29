#include <boost/program_options.hpp>
#include "compiler.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    grammar::compiler::CompilerOptions options = grammar::compiler::CompilerOptions();
    try {
        po::options_description desc("Compiler Options");
        desc.add_options()
            // Format: ("[--LongName, -shortName]", "Desc")
            ("help,h", "Print this help message") // help is triggered by --help or -h 
            ("parse-only,P", po::bool_switch(), "Stop after parsing") 
            ("symbol-collection-only,S", po::bool_switch(), "Stop after symbol collection") 
            ("peephole-only,e", po::bool_switch(), "Stop after peephole optimization")
            ("disable-peephole,E", po::bool_switch(), "disable peephole optimization") 
            ("type-check-only,T", po::bool_switch(), "Stop after type checking") 
            ("code-generation-only,C", po::bool_switch(), "Stop after code generation")
            ("register-allocation-only,R", po::bool_switch(), "Stop after register allocation")
            ("print-ast,p", po::bool_switch(), "Print the generated AST")
            ("print-input,i", po::bool_switch(), "Print the input before parsing")
            ("print-code-generation,c", po::bool_switch(), "Print the Intermediate code after the code generation phase")
            ("print-register-allocation,r", po::bool_switch(), "Print the Intermediate code after the register allocation phase")
            ("naive-register-allocation,n", po::bool_switch(), "Use the naive register allocation")
            ("input-file", po::value<std::string>(), "Input file to compile"); // this is to necessary
        po::positional_options_description p;
        // Here we say that we expect 1 non-posestional argument namely the file
        p.add("input-file", 1); 

        // vm holds the logic of the command-line i.e., the options 
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << "Usage: giga [options] <input-file>\n\n" 
                << desc; 
            return 0;
        }
        if (vm["parse-only"].as<bool>()) { 
            options.stop_after = grammar::compiler::StopAfterParser;
        }
        if (vm["symbol-collection-only"].as<bool>()) { 
            options.stop_after = grammar::compiler::StopAfterSymbolCollection;
        }
        if (vm["type-check-only"].as<bool>()) { 
            options.stop_after = grammar::compiler::StopAfterTypeCheck;
        }
        if (vm["code-generation-only"].as<bool>()) { 
            options.stop_after = grammar::compiler::StopAfterCodeGen;
        }
        if (vm["register-allocation-only"].as<bool>()) { 
            options.stop_after = grammar::compiler::StopAfterRegAlloc;
        }
        if (vm["peephole-only"].as<bool>()) { 
            options.stop_after = grammar::compiler::stopAfterPeepHole;
        }
        if (vm["print-ast"].as<bool>()) {
            options.print_ast = true;
        }
        if (vm["print-input"].as<bool>()) {
            options.print_input = true;
        }
        if (vm["disable-peephole"].as<bool>()) {
            options.use_peephole = true;
        }
        if (vm["print-code-generation"].as<bool>()) {
            options.print_code_generation = true;
        }
        if (vm["print-register-allocation"].as<bool>()) {
            options.print_register_allocation = true;
        }
        if (vm["naive-register-allocation"].as<bool>()) {
            options.naive_register_allocation = true;
        }
        if (vm.count("input-file")) {
            std::string_view filename = vm["input-file"].as<std::string>();
            grammar::compiler::compile_from_file(filename, options);
        } else {
            std::cerr << "Error: An input source is required.\n";
            return 1;
        }
    } catch (CompilerError &e) {
        std::cerr  << e.what() << std::endl;
        return 1;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return 1;
    }
    return 0;
}
