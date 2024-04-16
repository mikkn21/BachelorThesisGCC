#include "testing_main_lib.hpp"


void testMain(std::string flags, std::string filename, TestingOutcome expectedOutcome) {
    std::string filepath = "../tests/main_tests/" + filename;
    std::string shellCmd = "./giga " + flags + " " + filepath + " > /dev/null 2>&1";
    
    int status = system(shellCmd.c_str());

    // std::cout << "STATUS CODE: " << status << std::endl;

    if (expectedOutcome == TestingOutcome::SUCCESS) {
        BOOST_CHECK_MESSAGE(status == 0, "Giga Chad compiler failed to compile the file.");
    } else {
        BOOST_CHECK_MESSAGE(status != 0, "Giga Chad compiler succeeded to compile the file (but wasn't supposed to).");
    }
}


void testFileExists(std::string flags, std::string filename, TestingOutcome expectedOutcome) {
    // Remove old chad.s in case it exists
    std::string cleanup = "rm chad.s > /dev/null 2>/dev/null";
    system(cleanup.c_str());

    testMain(flags, filename, TestingOutcome::SUCCESS);

    // std::string target = "chad.s";
    // std::string findCmd = "find . -name " + target + " > /dev/null 2>&1";
    // int status = system(findCmd.c_str());
    int status = system(cleanup.c_str());

    std::cout << "STATUS CODE: " << status << std::endl;

    if (expectedOutcome == TestingOutcome::SUCCESS) {
        BOOST_CHECK_MESSAGE(status == 0, "The compiled file did not exist but was supposed to.");
    } else {
        BOOST_CHECK_MESSAGE(status != 0, "The compiled file exists (but wasn't supposed to).");
    }
}

std::string remove_whitespace(const std::string& str) {
    std::string result;
    remove_copy_if(str.begin(), str.end(), 
                        back_inserter(result),
                        ::isspace); 
    return result;
}

bool equal_ignore_whitespaces(const std::string& str1, const std::string& str2) {
    return (remove_whitespace(str1) == remove_whitespace(str2));
}

void testOutput(std::string flags, std::string filename, std::string expected_output, TestingOutcome expectedOutcome) {
    testMain(flags, filename, expectedOutcome);

    // compile assembly file called chad.s here, and run it to check the output is correct with expected_output
    std::string assembly_file = "chad.s";
    // Assemble the assembly file
    std::string assemble_cmd = "as --gstabs " + assembly_file + " -o chad.o";
    int assemble_result = system(assemble_cmd.c_str());
    BOOST_CHECK_EQUAL(assemble_result, 0);

    // Link the object file
    std::string link_cmd = "ld -o chad chad.o";
    int link_result = system(link_cmd.c_str());
    BOOST_CHECK_EQUAL(link_result, 0);

    // Run the compiled program and capture its output
    std::string run_cmd = "./chad";
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
}
