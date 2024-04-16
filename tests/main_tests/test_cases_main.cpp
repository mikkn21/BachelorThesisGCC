
#define BOOST_TEST_MODULE MainFunction
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_main_lib.hpp"


// Parser
BOOST_AUTO_TEST_CASE(ParseOnly) {testMain("-P", "parseOnly.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ParseOnlyLong) {testMain("--parse-only", "parseOnly.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ParseOnlyFail) {testMain("-S", "parseOnly.chad", TestingOutcome::FAILED);}

// Symbol collection 
BOOST_AUTO_TEST_CASE(SymbolCollectionOnly) {testMain("-S", "symbolcollOnly.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SymbolCollectionOnlyLong) {testMain("--symbol-collection-only", "symbolcollOnly.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SymbolCollectionOnlyFail) {testMain("", "symbolcollOnly.chad", TestingOutcome::FAILED);}

// Type checking 
BOOST_AUTO_TEST_CASE(TypeCheckingOnly) {testFileExists("-T", "typeCheckingOnly.chad", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeCheckingDontStop) {testFileExists("", "typeCheckingOnly.chad", TestingOutcome::SUCCESS);}

// Compile a program using main
BOOST_AUTO_TEST_CASE(EntireProgram) {testOutput("", "entireProgram.chad", "2", TestingOutcome::SUCCESS);}
