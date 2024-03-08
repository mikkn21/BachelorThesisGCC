#define BOOST_TEST_MODULE TypeCheck
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_type_check_lib.hpp" 


// Test VarDecl
BOOST_AUTO_TEST_CASE(TypeDoMatchExp) {testTypeCheckString("int x = 2;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchExpInt) {testTypeCheckString("int x = int;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoNOTMatchExp) {testTypeCheckString("int x = bool;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoNOTMatchExp2) {testTypeCheckString("bool x = 2;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoMatchExp3) {testTypeCheckString("bool x = true;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchExp4) {testTypeCheckString("bool x = false;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoNotMatchExpTrue) {testTypeCheckString("bool x = True;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoNotMatchExpFalse) {testTypeCheckString("bool x = False;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoNotMatchExpNegativeInt) {testTypeCheckString("int x = -123123123;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoMatchExpZero) {testTypeCheckString("int x = 0;\n", TestingOutcome::SUCCESS);}

// Test Exp + varDecl 
BOOST_AUTO_TEST_CASE(TypeMatchBinOp) {testTypeCheckString("int x = 2 + 2 ;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeNOTMatchBinOp) {testTypeCheckString("bool x = 2 + 2 ;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeNOTMatchExpPlusIntAndBool) {testTypeCheckString("bool x = 2 + true ;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeNOTMatchExpPlusIntAndBool2) {testTypeCheckString("bool x = int + true ;\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeMatchExpZero) {testTypeCheckString("int x = 0 + 0 ;\n", TestingOutcome::SUCCESS);}

// Boolean opeartors
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchEquals) {testTypeCheckString("bool x = 2 == 2; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchNotEquals) {testTypeCheckString("bool x = 3 != 2; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchAND) {testTypeCheckString("bool x = true & true; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchOR) {testTypeCheckString("bool x = true | true; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchGreaterThan) {testTypeCheckString("bool x = (4 + 4 ) > 5; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchGreaterThanEq) {testTypeCheckString("bool x = (4 + 4 ) >= 5; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchLessThan) {testTypeCheckString("bool x = (4 + 4 ) < 5; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchLessThanEq) {testTypeCheckString("bool x = (4 + 4 ) <= 5; ", TestingOutcome::SUCCESS);}

// Test BinOp
BOOST_AUTO_TEST_CASE(BinOpTypeMatch) {testTypeCheckString("2 + 2; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinOpTypeNOTMatch) {testTypeCheckString("true + 2;", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(BinOpTypeMatchComplex) {testTypeCheckString("( ((2*5/2) + 3) % 2 + 100 - 50) == 50; ", TestingOutcome::SUCCESS);}

// While
BOOST_AUTO_TEST_CASE(WhileGuardBoolTrue) {testTypeCheckString("while (true) {} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardBoolFalse) {testTypeCheckString("while (false) {} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardBoolComplex) {testTypeCheckString("while ((1 < 10 & 5 != 42) | true ) {} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardFailed) {testTypeCheckString("while (1 + 2) {} ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(WhileGuardFailed2) {testTypeCheckString("while (1) {} ", TestingOutcome::FAILED);}

// Test var_assign_def 
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignInt) {testTypeCheckString("while(true) { int x = 2; x = 3;  };\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignBool) {testTypeCheckString("while(true) { bool y = true; y = false;  };\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoNotMatchVarAssign) {testTypeCheckString("while(true) { int z = 2; z = false;  };\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignComplex) {testTypeCheckString("while(true) { int complex = 2; complex = (( 2 * 5) / 3 + (1+2+3+3+4) % 2) - 1 + 20;  };\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignNotOp) {testTypeCheckString("while(true) { bool mum = true; mum = mum & false;  };\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignDoubleVarible) {testTypeCheckString("while(true) { int dad = 3; int mum = 3; int child = 0; child = dad + mum;  };\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignNoSemicolon) {testTypeCheckString("while(true) { int x = 2; x = 3 }\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignOpAssign) {testTypeCheckString("while(true) { bool x = 2; x = 2 <= 5;  };\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignOpAssign2) {testTypeCheckString("while(true) { bool x = 2; x = 3 == 5;  };\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignOpAssign3) {testTypeCheckString("while(true) { bool x = 2; x = 3 >= 5;  };\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignFuncDecl) {testTypeCheckString("int f() { int x = 2; x = 3; }", TestingOutcome::SUCCESS);}

