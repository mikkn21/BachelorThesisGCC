#define BOOST_TEST_MODULE SymbolCollection
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_symbol_collection_lib.hpp"

BOOST_AUTO_TEST_CASE(RedeclareVariableInScopeFail) {testSymbolCollectionString("int f() { int x = 3; int x = 4; return 2;}\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(SimpleSucc) {testSymbolCollectionString("int f() { int x = 3; int y = 4; return x;}\n", TestingOutcome::SUCCESS);}