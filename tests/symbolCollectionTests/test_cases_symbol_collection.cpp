#define BOOST_TEST_MODULE SymbolCollection
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_symbol_collection_lib.hpp"


//  "if", "else", "while", "return", "print"
// Using reserved keywords for funcdecl
BOOST_AUTO_TEST_CASE(ReservedIfFuncFailF) {testSymbolCollectionString("int if() { return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReservedElseFuncFailF) {testSymbolCollectionString("int else() { return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReservedWhileFuncFailF) {testSymbolCollectionString("int while() { return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReservedReturnFuncFailF) {testSymbolCollectionString("int return() { return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReservedPrintFuncFailF) {testSymbolCollectionString("int print() { return 0;}", TestingOutcome::FAILED);}

// Using reserved keywords for vardecl
BOOST_AUTO_TEST_CASE(ReservedIfVarFailF) {testSymbolCollectionString("int if = 3;", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReservedElseVarFailF) {testSymbolCollectionString("int else = 3;", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReservedWhileVarFailF) {testSymbolCollectionString("int while = 3;", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReservedReturnVarFailF) {testSymbolCollectionString("int return = 3;", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReservedPrintVarFailF) {testSymbolCollectionString("int print = 3;", TestingOutcome::FAILED);}

BOOST_AUTO_TEST_CASE(DeclareNotInitYet) {testSymbolCollectionFile("../tests/symbolCollectionTests/declare_not_initialized.chad", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(DeclareOutofScope) {testSymbolCollectionFile("../tests/symbolCollectionTests/succTest1.chad", TestingOutcome::SUCCESS);}


BOOST_AUTO_TEST_CASE(RedeclareVariableInScopeFail) {testSymbolCollectionString("int f() { int x = 3; int x = 4; return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(SimpleSucc) {testSymbolCollectionString("int f() { int x = 3; int y = 4; return x;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncAlreadyInScopeFail) {testSymbolCollectionString("int f() {return 0;} int f() {return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(VarAlreadyInScopeFail) {testSymbolCollectionString("int x = 3; int x = 4;", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(VarNotDeclaredInScopeFail) {testSymbolCollectionString("int f() { int x = 3; return 0;} int g(){int y = 1 + x; return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(VarNotInitializedYetFail) {testSymbolCollectionString("int x = 0; int y = 5 + x;", TestingOutcome::SUCCESS);}



BOOST_AUTO_TEST_CASE(test) {testSymbolCollectionFile("../tests/symbolCollectionTests/test.chad", TestingOutcome::SUCCESS);}
