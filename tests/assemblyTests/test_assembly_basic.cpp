#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"


BOOST_AUTO_TEST_CASE(Precedence) {test_assembly_file("basic_chad_test", "test_precedence.chad","4\n4\n49\n38\n0\n7\n18\n18\n12\n337\ntrue");}
BOOST_AUTO_TEST_CASE(IfElse) {test_assembly_file("basic_chad_test", "test_if_else.chad","2\n3\n5\n7");}
