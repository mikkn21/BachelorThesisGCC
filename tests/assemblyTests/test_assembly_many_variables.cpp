#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"



BOOST_AUTO_TEST_CASE(ManyVariablesTest) {test_assembly_file("chad_many_variables_test", "test_many_variables.chad", "1\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13");}
