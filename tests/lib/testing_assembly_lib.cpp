#include "../../src/compiler.hpp"
#include "testing_parse_lib.hpp"

string remove_whitespace(const std::string& str) {
    string result;
    remove_copy_if(str.begin(), str.end(), 
                        back_inserter(result),
                        ::isspace); 
    return result;
}

bool equal_ignore_whitespaces(const std::string& str1, const std::string& str2) {
    return (remove_whitespace(str1) == remove_whitespace(str2));
}

template<typename Compiler>
void test_assembly(std::string output_file_name, std::string input, std::string expected_output, Compiler compiler) {
    cout << "\n----------------------" << endl;
    grammar::compiler::CompilerOptions options = grammar::compiler::CompilerOptions();
    // options.naive_register_allocation = false;
    options.use_peephole = false;

    std::string assembly_file = output_file_name + ".s";

    options.output_file = assembly_file;
    grammar::ast::Prog ast1;
    try {
        compiler(input, options); 
    } catch (const std::exception& e) {
        BOOST_CHECK_MESSAGE(false, "\n--- Caught exception: " << e.what() << "\n");
        return;
    } catch (...) {
        BOOST_CHECK_MESSAGE(false, "\n--- Caught unknown exception/error.\n");
        return;
    }
    // compile assembly file called chad.s here, and run it to check the output is correct with expected_output
    // Assemble the assembly file
    std::string assemble_cmd = "as --gstabs " + assembly_file + " -o " + output_file_name + ".o";
    int assemble_result = system(assemble_cmd.c_str());
    BOOST_CHECK_EQUAL(assemble_result, 0);

    // Link the object file
    std::string link_cmd = "ld -o " + output_file_name + " " + output_file_name + ".o";
    int link_result = system(link_cmd.c_str());
    BOOST_CHECK_EQUAL(link_result, 0);

    // Run the compiled program and capture its output
    std::string run_cmd = "./" + output_file_name;
    std::string output;
    FILE* pipe = popen(run_cmd.c_str(), "r");
    if (pipe) {
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr)
                output += buffer;
        }
        pclose(pipe);
    }

    // Compare the captured output with the expected output
    BOOST_CHECK_MESSAGE(equal_ignore_whitespaces(output, expected_output), "\n\n--- Expected output:\n" << expected_output << "\n\n--- Actual output:\n" << output << "\n\n");

    //Clean up the generated files
    remove(assembly_file.c_str());
    remove((output_file_name + ".o").c_str());
    remove(output_file_name.c_str());
}

void test_assembly_file(std::string output_file_name, std::string input, std::string expected_outpout) {
    std::string filepath = "../tests/assemblyTests/" + input;
    test_assembly(output_file_name, filepath, expected_outpout, grammar::compiler::compile_from_file);
    std::cout << "test finished: " << input << std::endl;
}

void test_assembly_string(std::string output_file_name, std::string input,std::string expected_outpout) {
    test_assembly(output_file_name, input, expected_outpout, grammar::compiler::compile_from_string);
}




