#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"



BOOST_AUTO_TEST_CASE(BetaAccessArray) {test_assembly_string("chad_beta_test", "int main(){int[3] arr; arr[2,4,5]; return 0;}", "Attempted to access a beta value on line 1");}
BOOST_AUTO_TEST_CASE(BetaAccessArrayExplicit) {test_assembly_string("chad_beta_test", "int main(){int[3] arr = beta; arr[2,4,5]; return 0;}", "Attempted to access a beta value on line 1");}
BOOST_AUTO_TEST_CASE(BetaAccessArrayExp) {test_assembly_string("chad_beta_test", "int main(){int[3] arr; int x = arr[2,3,4]; return 0;}", "Attempted to access a beta value on line 1");}
BOOST_AUTO_TEST_CASE(BetaAccessArrayLine2) {test_assembly_string("chad_beta_test", "int main(){int[3] arr;\n arr[2,4,5]; return 0;}", "Attempted to access a beta value on line 2");}
BOOST_AUTO_TEST_CASE(BetaAccessArrayLine4) {test_assembly_string("chad_beta_test", "int main(){int[3] arr;\n\n\n arr[2,4,5]; return 0;}", "Attempted to access a beta value on line 4");}

