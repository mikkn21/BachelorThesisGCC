#ifndef ASSEMBLY_TEST_LIB_HPP
#define ASSEMBLY_TEST_LIB_HPP


#include <string>
#include <boost/test/unit_test.hpp>

void test_assembly_file(std::string input, std::string expected_output);
void test_assembly_string(std::string input, std::string expected_output);

#endif // ASSEMBLY_TEST_LIB_HPP
