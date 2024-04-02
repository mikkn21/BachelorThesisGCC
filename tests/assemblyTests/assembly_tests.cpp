#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"


BOOST_AUTO_TEST_CASE(Print) {test_assembly_file("../tests/assemblyTests/test_print.chad", "1\n-1\n0\n1\n0\n9123923\n2\n-1\n1\n0");}
BOOST_AUTO_TEST_CASE(Binop) {test_assembly_file("../tests/assemblyTests/testBinop.chad", "13\n7\n30\n3\n1\n1\n0\n0\n1\n0\n1\n1\n1\n0\n1\n0");}
