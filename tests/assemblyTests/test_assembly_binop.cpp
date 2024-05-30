#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"



BOOST_AUTO_TEST_CASE(Binop) {test_assembly_file("chad_binop_test", "testBinop.chad", "13\n7\n30\n3\n1\ntrue\nfalse\nfalse\ntrue\nfalse\ntrue\ntrue\nfalse\ntrue\nfalse");}
