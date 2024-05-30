#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"

BOOST_AUTO_TEST_CASE(ArraysAsArguments) {test_assembly_file("chad_array_argument_test", "test_array_argument.chad","10\n80\n30\n90\n40\n50\n20\n60");}
