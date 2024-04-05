#ifndef ASSEMBLY_TEST_LIB_HPP
#define ASSEMBLY_TEST_LIB_HPP


#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/spirit/home/x3.hpp>

using namespace std;

void test_assembly_file(string input, string expected_output);
void test_assembly_string(string input, string expected_output);

#endif // ASSEMBLY_TEST_LIB_HPP
