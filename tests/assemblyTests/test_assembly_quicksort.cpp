#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"



BOOST_AUTO_TEST_CASE(QuickSort) {test_assembly_file("chad_quicksort_test", "quicksort.chad", "10\n20\n30\n40\n50\n60\n80\n90");}