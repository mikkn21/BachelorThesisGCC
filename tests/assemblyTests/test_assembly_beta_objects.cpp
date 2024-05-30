#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"

BOOST_AUTO_TEST_CASE(BetaArray) {test_assembly_string("chad_beta_object_test", "int main(){ int[1] arr; if arr == beta {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaArrayRev) {test_assembly_string("chad_beta_object_test", "int main(){ int[1] arr; if beta == arr {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaArray2) {test_assembly_string("chad_beta_object_test", "int main(){ int[1] arr = new int[10]; if arr != beta {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaArray2Rev) {test_assembly_string("chad_beta_object_test", "int main(){ int[1] arr = new int[10]; if beta != arr {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaClass) {test_assembly_string("chad_beta_object_test", "class T{} int main(){ T t; if t == beta {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaClassRev) {test_assembly_string("chad_beta_object_test", "class T{} int main(){ T t; if beta == t {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaClass2) {test_assembly_string("chad_beta_object_test", "class T{} int main(){ T t = new T(); if t != beta {print(0);} return 0; }", "0");}

