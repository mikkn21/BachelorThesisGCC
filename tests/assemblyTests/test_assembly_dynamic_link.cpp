#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"




BOOST_AUTO_TEST_CASE(DynamicLinking) {test_assembly_file("chad_dynamic_linking_test", "dynamic_linking.chad","3628800");}
