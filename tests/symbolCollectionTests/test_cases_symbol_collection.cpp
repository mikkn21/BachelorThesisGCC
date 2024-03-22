#define BOOST_TEST_MODULE SymbolCollection
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_symbol_collection_lib.hpp"


BOOST_AUTO_TEST_CASE(DeclareNotInitYet) {testSymbolCollectionFile("../tests/symbolCollectionTests/declare_not_initialized.chad", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(DeclareOutofScope) {testSymbolCollectionFile("../tests/symbolCollectionTests/succTest1.chad", TestingOutcome::SUCCESS);}


BOOST_AUTO_TEST_CASE(RedeclareVariableInScopeFail) {testSymbolCollectionString("int f() { int x = 3; int x = 4; return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(SimpleSucc) {testSymbolCollectionString("int f() { int x = 3; int y = 4; return x;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncAlreadyInScopeFail) {testSymbolCollectionString("int f() {return 0;} int f() {return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(VarAlreadyInScopeFail) {testSymbolCollectionString("int x = 3; int x = 4;", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(VarNotDeclaredInScopeFail) {testSymbolCollectionString("int f() { int x = 3; return 0;} int g(){int y = 1 + x; return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(VarNotInitializedYetFail) {testSymbolCollectionString("int x = 0; int y = 5 + x;", TestingOutcome::SUCCESS);}
