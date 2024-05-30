#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"




BOOST_AUTO_TEST_CASE(While) {test_assembly_file("chad_while_test", "test_while.chad","2\n3\n4\ntrue");}
BOOST_AUTO_TEST_CASE(NestedWhile) {test_assembly_file("chad_while_test", "nestedwhile.chad","0\n1\n2\n3");}
BOOST_AUTO_TEST_CASE(BreakContinue) {test_assembly_file("chad_while_test", "breakcont.chad","1\n3");}
