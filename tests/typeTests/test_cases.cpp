#define BOOST_TEST_MODULE TypeCheck
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_type_check_lib.hpp" 


// Test VarDecl
BOOST_AUTO_TEST_CASE(TypeDoMatchExp) {testTypeCheckString("int x = 2;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoNOTMatchExp) {testTypeCheckString("int x = bool;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoNOTMatchExp2) {testTypeCheckString("bool x = 2;\n", TestingOutcome::FAILED);}



// Test BinOp + varDecl 
BOOST_AUTO_TEST_CASE(TypeMatchBinOp) {testTypeCheckString("int x = 2 + 2 ;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeNOTMatchBinOp) {testTypeCheckString("int x = 2 + 2 ;\n", TestingOutcome::FAILED);}


// Test BinOp
BOOST_AUTO_TEST_CASE(BinOpTypeMatch) {testTypeCheckString("2 + 2 ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinOpTypeNOTMatch) {testTypeCheckString("true + 2", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(BinOpTypeMatchComplex) {testTypeCheckString("( ((2*5/2) + 3) % 2 + 100 - 50) == 50 ", TestingOutcome::SUCCESS);}



// While
BOOST_AUTO_TEST_CASE(WhileGuardBoolTrue) {testTypeCheckString("while (true) {} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardBoolFalse) {testTypeCheckString("while (false) {} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardBoolComplex) {testTypeCheckString("while ((1 < 10 & 5 != 42) | true ) {} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardFailed) {testTypeCheckString("while (1 + 2) {} ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(WhileGuardFailed2) {testTypeCheckString("while (1) {} ", TestingOutcome::FAILED);}

