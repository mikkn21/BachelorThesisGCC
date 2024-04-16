#ifndef MAIN_TEST_LIB_HPP
#define MAIN_TEST_LIB_HPP


#include <string>
#include <boost/test/unit_test.hpp>

enum class TestingOutcome {
    SUCCESS,
    FAILED
};

/// @brief Tests the main function of the Giga Chad compiler.
/// @param flags The flags to compile with.
/// @param filename File path is always the same, thus only the filename is needed
void testMain(std::string flags, std::string filename, TestingOutcome expectedOutcome);

/// @brief Tests if the compiled file i.e., chad.s exists after compilation
/// @param flags The flags to compile with.
/// @param filename File path is always the same, thus only the filename is needed
/// @param expectedOutcome The expected outcome of the test
void testFileExists(std::string flags, std::string filename, TestingOutcome expectedOutcome);

/// @brief Tests the output of the compiled file against the expected output
/// @param flags The flags to compile  with.
/// @param filename File path is always the same, thus only the filename is needed
/// @param expectedOutput The expected output of the compiled file
/// @param expectedOutcome The expected outcome of the test
void testOutput(std::string flags, std::string filename, std::string expectedOutput, TestingOutcome expectedOutcome);

#endif // MAIN_TEST_LIB_HPP
