#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"




BOOST_AUTO_TEST_CASE(BasicStructs) {test_assembly_file("chad_structs_test1", "basic_structs.chad","0\n3\n0\n0\n3\n0\n5");}
BOOST_AUTO_TEST_CASE(AdvancedStructs) {test_assembly_file("chad_structs_test2", "advanced_structs.chad","1\n2\n3\n4\n5");}
BOOST_AUTO_TEST_CASE(StructsAsArguments) {test_assembly_file("chad_structs_test3", "test_struct_argument.chad","1\n2\n3");}
BOOST_AUTO_TEST_CASE(ReturnStructs) {test_assembly_file("chad_structs_test4", "return_structs.chad","1\n2\n3\n4\n5");}
BOOST_AUTO_TEST_CASE(StructOutsideScope) {test_assembly_string("chad_structs_test5", "class T{int x;} int main(){ T t = new T(); t.x = 2; int f(){print(t.x); return 0;} f(); return 0; }", "2");}
BOOST_AUTO_TEST_CASE(ClassesReturn) {test_assembly_string("chad_structs_class_return", "class T{int x;} T f() {T t = new T(); t.x = 42;  return t;} int main() { T e = f(); print(e.x); return 0; }", "42");}

