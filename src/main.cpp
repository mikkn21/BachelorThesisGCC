#include <boost/program_options.hpp>
#include "compiler.hpp"


using namespace std;
using namespace grammar::compiler;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    CompilerOptions options = CompilerOptions();
    try {
        po::options_description desc("Compiler Options");
        desc.add_options()
            // Format: ("[--LongName, -shortName]", "Desc")
            ("help,h", "Print this help message") // help is triggered by --help or -h 
            ("parse-only,P", po::bool_switch(), "Stop after parsing") 
            ("symbol-collection-only,S", po::bool_switch(), "Stop after symbol collection") 
            ("type-check-only,T", po::bool_switch(), "Stop after type checking") 
            ("code-generation-only,C", po::bool_switch(), "Stop after code generation")
            ("register-allocation-only,R", po::bool_switch(), "Stop after register allocation")
            ("print-ast,p", po::bool_switch(), "Print the generated AST")
            ("print-input,i", po::bool_switch(), "Print the input before parsing")
            ("print-code-generation,c", po::bool_switch(), "Print the Intermediate code after the code generation phase")
            ("print-register-allocation,r", po::bool_switch(), "Print the Intermediate code after the register allocation phase")
            ("input-file", po::value<string>(), "Input file to compile"); // this is to necessary
        po::positional_options_description p;
        // Here we say that we expect 1 non-posestional argument namely the file
        p.add("input-file", 1); 

        // vm holds the logic of the command-line i.e., the options 
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << "Usage: giga [options] <input-file>\n\n" 
                << desc; 
            return 0;
        }
        if (vm["parse-only"].as<bool>()) { 
            options.stopAfter = StopAfterParser;
        }
        if (vm["symbol-collection-only"].as<bool>()) { 
            options.stopAfter = StopAfterSymbolCollection;
        }
        if (vm["type-check-only"].as<bool>()) { 
            options.stopAfter = StopAfterTypeCheck;
        }
        if (vm["code-generation-only"].as<bool>()) { 
            options.stopAfter = StopAfterCodeGen;
        }
        if (vm["register-allocation-only"].as<bool>()) { 
            options.stopAfter = StopAfterRegAlloc;
        }
        if (vm["print-ast"].as<bool>()) {
            options.printAst = true;
        }
        if (vm["print-input"].as<bool>()) {
            options.printInput = true;
        }
        if (vm["print-code-generation"].as<bool>()) {
            options.printCodeGeneration = true;
        }
        if (vm["print-register-allocation"].as<bool>()) {
            options.printRegisterAllocation = true;
        }
        if (vm.count("input-file")) {
            string_view filename = vm["input-file"].as<string>();
            grammar::compiler::compileFromFile(filename, options);
        } else {
            cerr << "Error: An input source is required.\n";
            return 1;
        }
    } catch (CompilerError &e) {
        cerr  << e.what() << endl;
        return 1;
    } catch (exception &e) {
        cerr << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown error" << endl;
        return 1;
    }
    return 0;
}
