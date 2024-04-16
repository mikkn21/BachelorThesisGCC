#ifndef MAIN_TEST_LIB_HPP
#define MAIN_TEST_LIB_HPP


#include <string>
#include <boost/test/unit_test.hpp>

enum class TestingOutcome {
    SUCCESS,
    FAILED
};

/// @brief Tests the main function of the Giga Chad compiler.
/// @param flags The flags to compile the file with.
/// @param filename File path is always the same, thus only the filename is needed
void testMain(std::string flags, std::string filename, TestingOutcome expectedOutcome);
void testFileExists(std::string flags, std::string filename, TestingOutcome expectedOutcome);
void testOutput(std::string flags, std::string filename, std::string expected_output, TestingOutcome expectedOutcome);

#endif // MAIN_TEST_LIB_HPP
