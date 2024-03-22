#define BOOST_TEST_MODULE TypeCheck
#define BOOST_TEST_DYN_LINK  


#include "../lib/testing_type_check_lib.hpp" 

// BOOST_AUTO_TEST_CASE(TEMP) {BOOST_TEST(2 == 2);}

// Test VarDecl
BOOST_AUTO_TEST_CASE(TypeDoMatchExp) {testTypeCheckString("int main() {int x = 2; return 0;}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoNOTMatchExp) {testTypeCheckString("int main() {int x = false; return 0;}\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoNOTMatchExp2) {testTypeCheckString("int main() {bool x = 2; return 0;}\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoMatchExp3) {testTypeCheckString("int main() {bool x = true; return 0;}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchExp4) {testTypeCheckString("int main() {bool x = false; return 0; }\n", TestingOutcome::SUCCESS);} 
BOOST_AUTO_TEST_CASE(TypeDoNotMatchExpNegativeInt) {testTypeCheckString("int main() { int x = -123123123; return 0;}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchExpZero) {testTypeCheckString("int main() {int x = 0; return 0;}\n", TestingOutcome::SUCCESS);}

// Test Exp + varDecl 
BOOST_AUTO_TEST_CASE(TypeMatchBinOp) {testTypeCheckString("int main() { int x = 2 + 2 ; return 0;}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeNOTMatchBinOp) {testTypeCheckString("int main() { bool x = 2 + 2 ; return 0;}\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeNOTMatchExpPlusIntAndBool) {testTypeCheckString("int main() { bool x = 2 + true ; return 0;}\n", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeMatchExpZero) {testTypeCheckString("int main() { int x = 0 + 0 ; return 0; }\n", TestingOutcome::SUCCESS);}

// Boolean opeartors
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchEquals) {testTypeCheckString("int main() { bool x = 2 == 2; return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchNotEquals) {testTypeCheckString("int main() { bool x = 3 != 2; return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchAND) {testTypeCheckString("int main() { bool x = true & true; return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchOR) {testTypeCheckString("int main() { bool x = true | true; return 0;} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchGreaterThan) {testTypeCheckString("int main() { bool x = (4 + 4 ) > 5; return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchGreaterThanEq) {testTypeCheckString("int main() { bool x = (4 + 4 ) >= 5; return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchLessThan) {testTypeCheckString("int main() { bool x = (4 + 4 ) < 5; return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BoolOpTypeMatchLessThanEq) {testTypeCheckString("int main() { bool x = (4 + 4 ) <= 5; return 0; } ", TestingOutcome::SUCCESS);}

// Test StatementExpression
BOOST_AUTO_TEST_CASE(StatementExpressionTypeMatch) {testTypeCheckString("int main() { int f() {2 + 2; return 2;} return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(StatementExpressionTypeNOTMatch) {testTypeCheckString("int main() { int f() {true + 2; return 2;} return 0; }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(StatementExpressionMatchComplex) {testTypeCheckString("int main() { int f () {( ((2*5/2) + 3) % 2 + 100 - 50) == 50; return 2;} return 0; } ", TestingOutcome::SUCCESS);}


// Test Return
BOOST_AUTO_TEST_CASE(ReturnFuncFalse) {testTypeCheckString("int main() { int f() { return false;} return 0; }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ReturnFuncTrue) {testTypeCheckString("int main() { int f() { return 2;} return 0; }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ReturnVarSimple) {testTypeCheckString("int main() { int f() { int x = 2; return x;} return 0; }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ReturnDeadCode) {testTypeCheckString("int main() { int f() { int x = 2; return x; x = 40 + 2; } return 0; }", TestingOutcome::FAILED);}



// While
BOOST_AUTO_TEST_CASE(WhileGuardBoolTrue) {testTypeCheckString("int main() { int f() {while (true) {} return 2;} return 0;} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardBoolFalse) {testTypeCheckString("int main() { int f() {while (false) {} return 2;} return 0;} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardBoolComplex) {testTypeCheckString("int main() { int f() {while ((1 < 10) & (5 != 42) | true ) {} return 2;} return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(WhileGuardFailed) {testTypeCheckString("int main() { int f() {while (1 + 2) {} return 2;} return 0; } ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(WhileGuardFailed2) {testTypeCheckString("int main() { int f() {while (1) {} return 2;} return 0;} ", TestingOutcome::FAILED);}

// Test var_assign_def 
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignInt) {testTypeCheckString("int main() { int f() {while(true) { int x = 2; x = 3; } return 2;} return 0; }\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignBool) {testTypeCheckString("int main() { int f() {while(true) { bool y = true; y = false;  } return 2;} return 0; }\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoNotMatchVarAssign) {testTypeCheckString("int main() { int f() {while(true) { int z = 2; z = false;  } return 2;} return 0; }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignComplex) {testTypeCheckString("int main() { int f() {while(true) { int complex = 2; complex = (( 2 * 5) / 3 + (1+2+3+3+4) % 2) - 1 + 20;  } return 2;} return 0; }\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignNotOp) {testTypeCheckString("int main() {while(true) { bool mum = true; mum = mum & false;  } return 2;}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignDoubleVarible) {testTypeCheckString("int main() {while(true) { int dad = 3; int mum = 3; int child = 0; child = dad + mum;  } return 2;}\n", TestingOutcome::SUCCESS);}


BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignOpAssign) {testTypeCheckString("int main() {while(true) { bool x = true; x = 2 <= 5;  } return 2;} ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignOpAssign2) {testTypeCheckString("int main() {while(true) { bool x = true; x = 3 == 5;  } return 2;}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignOpAssign3) {testTypeCheckString("int main() {while(true) { bool x = true; x = 3 >= 5;  } return 2;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeDoMatchVarAssignFuncDecl) {testTypeCheckString("int main() { int x = 2; x = 3; return 2; }", TestingOutcome::SUCCESS);}

BOOST_AUTO_TEST_CASE(TypeCheckMainReturnTypeZero) {testTypeCheckString("int main() { int x = 2; return 0;  }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeCheckMainReturnTypeVar) {testTypeCheckString("int main() { int x = 2; return x;  }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TypeCheckMainReturnTypeNotInt) {testTypeCheckString("bool main() {return true;  }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeCheckMainReturnTypeNotInt2) {testTypeCheckString("bool main() {return 0;  }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TypeCheckMainWithParameters) {testTypeCheckString("int main(int x, int y) {return 0;  }", TestingOutcome::FAILED);}

BOOST_AUTO_TEST_CASE(DeclareNotInitYet) {testTypeCheckFile("../tests/typeTests/shadow.chad", TestingOutcome::SUCCESS);}
