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

// Print
BOOST_AUTO_TEST_CASE(PrintFunc) {testTypeCheckString("int main() { int f() { print(2); return 0;} return 0; }", TestingOutcome::SUCCESS);}

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

// Test var decl statement
BOOST_AUTO_TEST_CASE(VarDeclStatementTest) {testTypeCheckString("int main() {int x; x = 2; return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(VarDeclStatementTest1) {testTypeCheckString("int main() {int x; return x;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(VarDeclStatementTest2) {testTypeCheckString("int main() {bool x; return x;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(VarDeclStatementTest3) {testTypeCheckString("int main() {int x; bool y = x; return 0;}", TestingOutcome::FAILED);}

// Parameters
BOOST_AUTO_TEST_CASE(Parameter) {testTypeCheckString("int main() {int f(int x) { return x; } return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TwoParameters) {testTypeCheckString("int main() {int f(int x, int y) { return x; } return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ParameterWrongType) {testTypeCheckString("int main() {int f(bool x) { int y = x; return 0; } return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ParameterWrongTypeInReturn) {testTypeCheckString("int main() {int f(bool x) { return x; } return 0;}", TestingOutcome::FAILED);}

// Arguments
BOOST_AUTO_TEST_CASE(Argument) {testTypeCheckString("int main() {int f(int x) { return 0; } f(2); return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArgumentWrong) {testTypeCheckString("int main() {int f(int x) { return 0; } f(true); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TooFewArguments) {testTypeCheckString("int main() {int f(int x) { return 0; } f(); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TooFewArguments2) {testTypeCheckString("int main() {int f(int x, int y) { return 0; } f(2); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TooManyArguments) {testTypeCheckString("int main() {int f(int x) { return 0; } f(1, 2, 3 , 4, 5); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TooManyArgument2) {testTypeCheckString("int main() {int f(int x, int y) { return 0; } f(1, 2, 3 , 4, 5); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TwoArguments) {testTypeCheckString("int main() {int f(int x, int y) { return 0; } f(2, 2); return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TwoArgumentsWrong1) {testTypeCheckString("int main() {int f(int x, int y) { return 0; } f(2, true); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TwoArgumentsWrong2) {testTypeCheckString("int main() {int f(int x, int y) { return 0; } f(true, 2); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TwoArgumentsWrong3) {testTypeCheckString("int main() {int f(int x, int y) { return 0; } f(false, true); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TwoArgumentsDifferentTypes) {testTypeCheckString("int main() {int f(int x, bool y) { return 0; } f(2, true); return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TwoArgumentsDifferentTypesWrong1) {testTypeCheckString("int main() {int f(int x, bool y) { return 0; } f(true, 2); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TwoArgumentsDifferentTypesWrong2) {testTypeCheckString("int main() {int f(int x, bool y) { return 0; } f(2, 2); return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(TwoArgumentsDifferentTypesWrong3) {testTypeCheckString("int main() {int f(int x, bool y) { return 0; } f(true, true); return 0;}", TestingOutcome::FAILED);}


BOOST_AUTO_TEST_CASE(VarDeclaredAfterUsed) {testTypeCheckString("int main() {int f(int x){ return x; } int y = f(x); int x = 2; return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(FuncCalledBeforeDeclared) {testTypeCheckString("int main() {int y = f(4); int f(int x){ return x; } return 0;}", TestingOutcome::SUCCESS);}



BOOST_AUTO_TEST_CASE(DeclareNotInitYet) {testTypeCheckFile("../tests/typeTests/shadow.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ParameterFunctionCall) {testTypeCheckFile("../tests/typeTests/functionParam.chad", TestingOutcome::SUCCESS);}


// if 
BOOST_AUTO_TEST_CASE(SimpleIfVarWorks) {testTypeCheckString("int main() { int x = 2; if(x == 2 ) {}  return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfWorks) {testTypeCheckString("int main() { if true {}  return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfWorks2) {testTypeCheckString("int main() { if false {} else {}  return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfNotWorks) {testTypeCheckString("int main() { if 1 {}  return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ComplexIfGuard) {testTypeCheckString("int main() { int x = 2; if x != 24 | (x < 4 & x == 2 ) {} else {}  return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ComplexIfGuardFails) {testTypeCheckString("int main() { int x = 2; if x != 24 | (0 & x == 2) {} else {}  return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(SimpleIfGuardFails) {testTypeCheckString("int main() { int x = 2; if x + 4 {} else {}  return 0;}", TestingOutcome::FAILED);}

// Precedence checks
BOOST_AUTO_TEST_CASE(BooleanOperators1) {testTypeCheckString("int main() { bool x = true & 2 == 4; return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BooleanOperators2) {testTypeCheckString("int main() { bool x = 2 == 4 & true; return 0;}", TestingOutcome::SUCCESS);}

BOOST_AUTO_TEST_CASE(BooleanOperators3) {testTypeCheckString("int main() { bool x = 2 > 3 | 2 == 2; return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BooleanOperators4) {testTypeCheckString("int main() { bool x = 2 == 2 | 2 > 3; return 0;}", TestingOutcome::SUCCESS);}

BOOST_AUTO_TEST_CASE(BooleanOperators5) {testTypeCheckString("int main() { bool x = 2 > 3 & 2 == 2; return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BooleanOperators6) {testTypeCheckString("int main() { bool x = 2 == 2 & 2 > 3; return 0;}", TestingOutcome::SUCCESS);}


// array 
BOOST_AUTO_TEST_CASE(ArrayDimNotCorrect) {testTypeCheckString("int main() { int[2] x = new int[5,2,2]; return 0; }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ArrayIndexDimNotCorrect) {testTypeCheckString("int main() { int[3] a = new int[5,5]; return 0; }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ArrayIndexDimNotCorrect1) {testTypeCheckString("int main() { int[3] a = new int[5,5,5]; int y = a[2,3,5,5]; return 0; }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ArrayIndexDimNotCorrect2) {testTypeCheckString("int main() { int[3] a = new int[5,5]; int y = a[2,3]; return 0; } ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ArrayTypeMismatch) {testTypeCheckString("int main() { int[1] x = new bool[5]; return 0; }", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ArrayTypeMatch) {testTypeCheckString("int main() { bool[1] x = new bool[5]; return 0; }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayIndex) {testTypeCheckString("int main() { int[2] a = new int[5,5]; a[1,3] = 2;  int x = a[1,3]; return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayIndex15) {testTypeCheckString("int main() { int[1] a = new int[15]; a[14] = 2;  return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayIndex16) {testTypeCheckString("int main() { int[1] a = new int[15]; int x = a[14];  return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayIndex2) {testTypeCheckString("int main() { int[2] a = new int[5,5]; a[1] = 2;  int x = a[10]; return 0; } ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ArrayIndex3) {testTypeCheckString("int main() { int[2] a = new int[5,5]; a[1] = 2;  int x = a[1,2]; return 0; } ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ArrayMixedTypes) {testTypeCheckString("int main() { int[3] a = new int[1,2, true]; return 0; } ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ArrayMixedTypes2) {testTypeCheckString("int main() { int[1] a = new int[20]; a[15] = true; return 0; } ", TestingOutcome::FAILED);}

BOOST_AUTO_TEST_CASE(IllegalIndexingWithFunction) {testTypeCheckString("int main() { int f () { return 0; } f[2]; return 0; } ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(IllegalIndexingWithFunction2) {testTypeCheckString("int main() { int f () { return 0; } f()[2]; return 0; } ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(IllegalIndexingWithClass) {testTypeCheckString("class T { } int main() { T[1]; return 0; } ", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(IndexingWithClass) {testTypeCheckString("class T { int x; } int main() { int[1] a = new int[5]; T b = new T(); a[b.x]; return 0; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(IndexingWithClass2) {testTypeCheckString("class T { int x; } int main() { int[1] a = new int[5]; T b = new T(); int x = a[b.x]; return 0; } ", TestingOutcome::SUCCESS);}



BOOST_AUTO_TEST_CASE(ArrayFromFile) {testTypeCheckFile("../tests/typeTests/arrayStuff.chad", TestingOutcome::FAILED);}


// Classes
BOOST_AUTO_TEST_CASE(ClassNotDeclared) {testTypeCheckString("int main() { T a = new T();  return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(NewClassArguments) {testTypeCheckString("int main() { class T { int x; } T a = new T(42);  return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(SimpleClass) {testTypeCheckString("int main() { class T { } T a = new T(); return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleClass2) {testTypeCheckString("class T { } int main() { T a = new T(); return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleClass3) {testTypeCheckString("int main() { T a = new T(); class T { } return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleClass4) {testTypeCheckString("int main() { T a = new T(); return 0;} class T { }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ClassNotExist) {testTypeCheckString("int main() { T a; return 0;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(ClassNotExist2) {testTypeCheckString("int main() { T a = new T(); return 0;}", TestingOutcome::FAILED);}

// Dot operator 
BOOST_AUTO_TEST_CASE(SimpleDot) {testTypeCheckString("int main() { class T { int x; } T a = new T(); return a.x;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleDot2) {testTypeCheckString("int main() { class T { E b; } class E { int x; } T a = new T(); return a.b.x;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleDot3) {testTypeCheckString("int main() { class T { bool x; } T a = new T(); bool y = a.x; return 0;}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ReturnDotOp) {testTypeCheckString("int main() { class T { int x; } T a = new T(); return a.x; }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncWithClassProperty) {testTypeCheckString("class T {int y; } int main() { int f(int x) { return x; } T a = new T(); int x = f(a.y);  return a.y; } ", TestingOutcome::SUCCESS);}


// BOOST_AUTO_TEST_CASE(DotOpClassType) {testTypeCheckString(" class G { int y; } class T { G x; } T a = new T(); a.x.y = 2; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleDotFail) {testTypeCheckString("int main() { class T { int x; } T a = new T(); return a.y;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(SimpleDotFail2) {testTypeCheckString("int main() { class T {} T a = new T(); return a.x;}", TestingOutcome::FAILED);}
BOOST_AUTO_TEST_CASE(SimpleDotFail3) {testTypeCheckString("int main() { class T { int x; } T a = new T(); return a.x.y;}", TestingOutcome::FAILED);}
