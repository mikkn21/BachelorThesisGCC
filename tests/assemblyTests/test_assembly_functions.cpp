#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"



BOOST_AUTO_TEST_CASE(FunctionCall) {test_assembly_file("chad_function_test", "test_function_call.chad","5");}
BOOST_AUTO_TEST_CASE(FunctionReturn) {test_assembly_file("chad_function_test", "test_function_return.chad","6");}
BOOST_AUTO_TEST_CASE(FunctionParamter) {test_assembly_file("chad_function_test", "test_function_parameters.chad","12");}