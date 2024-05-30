#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"


BOOST_AUTO_TEST_CASE(StaticLinkingRead) {test_assembly_file("chad_static_link_test", "static_linking_read.chad","1\n1\n2\n5\n6\n7\n8\n9");}
BOOST_AUTO_TEST_CASE(StaticLinkingWrite) {test_assembly_file("chad_static_link_test", "static_linking_write.chad","0\n10\n20\n50\n11");}
