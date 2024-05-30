#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"




BOOST_AUTO_TEST_CASE(BasicStructs) {test_assembly_file("chad_structs_test", "basic_structs.chad","0\n3\n0\n0\n3\n0\n5");}
BOOST_AUTO_TEST_CASE(AdvancedStructs) {test_assembly_file("chad_structs_test", "advanced_structs.chad","1\n2\n3\n4\n5");}
// BOOST_AUTO_TEST_CASE(StructsAsArguments) {test_assembly_file("chad_structs_test", "struct_as_argument.chad","1\n2\n3");}  // TODO: Fix this test
// BOOST_AUTO_TEST_CASE(ReturnStructs) {test_assembly_file("chad_structs_test", "return_structs.chad","1\n2\n3\n4\n5");} //TODO: Fix this test
BOOST_AUTO_TEST_CASE(StructOutsideScope) {test_assembly_string("chad_structs_test", "class T{int x;} int main(){ T t = new T(); t.x = 2; int f(){print(t.x); return 0;} f(); return 0; }", "2");}

