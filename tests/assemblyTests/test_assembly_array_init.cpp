#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"

BOOST_AUTO_TEST_CASE(ArraysInit) {test_assembly_file("chad_array_init_test", "test_array_init.chad","0\n0\n0\n0");}
