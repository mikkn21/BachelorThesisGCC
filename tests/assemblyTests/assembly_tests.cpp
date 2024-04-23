#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"


BOOST_AUTO_TEST_CASE(Print) {test_assembly_file("test_print.chad", "1\n0\n1\n0\n9123923\n2\n1\n0");}
BOOST_AUTO_TEST_CASE(Binop) {test_assembly_file("testBinop.chad", "13\n7\n30\n3\n1\n1\n0\n0\n1\n0\n1\n1\n0\n1\n0");}
BOOST_AUTO_TEST_CASE(Precedence) {test_assembly_file("test_precedence.chad","4\n4\n49\n38\n0\n7\n18\n18\n12\n337\n1");}
BOOST_AUTO_TEST_CASE(FunctionCall) {test_assembly_file("test_function_call.chad","5");}
BOOST_AUTO_TEST_CASE(FunctionReturn) {test_assembly_file("test_function_return.chad","6");}
BOOST_AUTO_TEST_CASE(FunctionParamter) {test_assembly_file("test_function_parameters.chad","12");}
BOOST_AUTO_TEST_CASE(IfElse) {test_assembly_file("test_if_else.chad","2\n3\n5\n7");}
BOOST_AUTO_TEST_CASE(While) {test_assembly_file("test_while.chad","2\n3\n4\n1");}
BOOST_AUTO_TEST_CASE(NestedWhile) {test_assembly_file("nestedwhile.chad","0\n1\n2\n3");}
BOOST_AUTO_TEST_CASE(BreakContinue) {test_assembly_file("breakcont.chad","1\n3");}
BOOST_AUTO_TEST_CASE(DynamicLinking) {test_assembly_file("dynamic_linking.chad","3628800");}
BOOST_AUTO_TEST_CASE(StaticLinking) {test_assembly_file("static_linking.chad","2\n2\n0\n9\n1\n2\n3\n4");}



