#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"




BOOST_AUTO_TEST_CASE(Arrays) {test_assembly_file("array_chad", "test_arrays.chad","42\n17\n29\n8");}

