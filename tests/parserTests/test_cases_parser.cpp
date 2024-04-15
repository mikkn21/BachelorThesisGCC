#define BOOST_TEST_MODULE Parse
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_parse_lib.hpp"

 
// Test going through primitive_type, type, id and var_decl and some of expression_base:
BOOST_AUTO_TEST_CASE(VarDeclIntIdSucc) {test_parse_string("int my_name123 = 2;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(VarDeclBoolIdSucc) {test_parse_string("int my_name123 = true;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(VarDeclBool2IdSucc) {test_parse_string("int my_name123 = false;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(VarDeclBoolTypeSucc) {test_parse_string("bool my_name123 = true;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(VarDeclIdFail2) {test_parse_string("int 0name123 = 2;\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(VarDeclMissingEqualSign) {test_parse_string("int 0name123 2;\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(VarDeclMissingType) {test_parse_string("name = 2;\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(VarDeclMissingSemiColon) {test_parse_string("int 0name123 = 2;\n", TestingOutcome::PARSE_FAILED);}

// Test going through expression_base
BOOST_AUTO_TEST_CASE(ExpBaseInvalidAssignment) {test_parse_string("int name = 2a;\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesis) {test_parse_string("int k = (2 + 5);\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ExpParanthesisNestedArithmetic) {test_parse_string("int result = (1 + (2 * 3));\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ExpParanthesisMissingStartBracket) {test_parse_string("int result = (1 + 2 * 3));\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisMissingEndBracket) {test_parse_string("int result = ((1) + (2 * (3));\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisEmpty) {test_parse_string("int result = ();\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisEmpty2) {test_parse_string("int result = (1 + (2 * ()3);\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisOther) {test_parse_string("int result = (1 + (2 (*) 3);\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisOther2) {test_parse_string("(int result = (1 + (2 * 3));\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisOther3) {test_parse_string("int result (=) (1 + (2 * 3);\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisOther4) {test_parse_string("(int result =) (1 + (2 * 3);\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisOther5) {test_parse_string("int (result) = (1 + (2 * 3);\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisOther6) {test_parse_string("(int) result = (1 + (2 * 3);\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ExpParanthesisRedudant) {test_parse_string("int result = ((((1 + (2 * 3)))));\n", TestingOutcome::SUCCESS);}


// Tests going through binop_exp
BOOST_AUTO_TEST_CASE(BinopOperationPlus) {test_parse_string("int x = 10 + 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationMinus) {test_parse_string("int x = 10 - 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationMultiplication) {test_parse_string("int x = 10 * 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationDivision) {test_parse_string("int x = 10 / 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationModulos) {test_parse_string("int x = 10 % 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationAnd) {test_parse_string("int x = 10 & 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationOr) {test_parse_string("int x = 10 | 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationNotEqual) {test_parse_string("int x = 10 != 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationEqual) {test_parse_string("int x = 10 == 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationGreater) {test_parse_string("int x = 10 > 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationGreaterEqual) {test_parse_string("int x = 10 >= 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationLesser) {test_parse_string("int x = 10 < 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationLesserEqual) {test_parse_string("int x = 10 <= 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationInvalidOperator) {test_parse_string("int x = 10 ? 10;\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(BinopOperationMultipleBinaryOperators) {test_parse_string("int x = 10 + 10 * 10 / 10;\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(BinopOperationInvalidNoBinaryOperator) {test_parse_string("int x = 10  10;\n", TestingOutcome::PARSE_FAILED);}




// Test going through parameter, parameter_list, func_decl, block and block_line:
BOOST_AUTO_TEST_CASE(FuncDeclIntSucc) {test_parse_string("int f (int g, int h) {\n}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncDeclBoolSucc) {test_parse_string("int f (bool g, bool h) {\n}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncDeclEmptySucc) {test_parse_string("int f () {\n}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncDeclInputFail) {test_parse_string("int f (f int, ttt tttt) {\n}\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(FuncDeclInput2Fail) {test_parse_string("int f (f int, f bool) {\n}\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(FuncDeclParameterMissingName) {test_parse_string("int f (int) {\n}\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(FuncDeclParameterMissingType) {test_parse_string("int f (name) {\n}\n", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(FuncDeclBlockEmptySucc) {test_parse_string("int f () {\n}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(VarDeclInFuncDeclSucc) {test_parse_string("int f () {\nint x = 2;\n}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncDeclInFuncDeclSucc) {test_parse_string("int f () {\nbool g () {\n}\n}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncDeclFuncDeclSucc) {test_parse_string("int f () {\n}\nint g () {\n}\n", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(FuncMultilinerVarDecl) {test_parse_string("int f (int k) {\nint x = 2;\nbool l = 2;\n}\n", TestingOutcome::SUCCESS);}
//BOOST_AUTO_TEST_CASE(FuncMixedMultiliner) {test_parse_file("../tests/parserTests/FuncMixedMultiliner.txt", TestingOutcome::SUCCESS);} // WHY DOES THIS NOT WORK?
BOOST_AUTO_TEST_CASE(FuncMixedMultiliner) {test_parse_string(
    R"(int f (int k) {
int x = 2;
bool g () {
}
bool l = 2;
int f (int k, bool v) {
bool isTrue = false;
}
}
)"
, TestingOutcome::SUCCESS);}


// Test going prog and decl:
BOOST_AUTO_TEST_CASE(MultipleDecl) {test_parse_string(
R"(int f () {
}
int g () {
}
int k = 3;
)", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(Nothing) {test_parse_string("", TestingOutcome::SUCCESS);}


// Test going Statement
BOOST_AUTO_TEST_CASE(StatementVarAssignSimple) {test_parse_string("int f () {\nx = 10;\n}\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementVarAssignmentDoubleSemicolon) {test_parse_string("int f () {\nx = 10;;\n}\n", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(StatementVarWrongAssignment) {test_parse_string("int f () {\n2 = 10;\n}\n", TestingOutcome::PARSE_FAILED); }

BOOST_AUTO_TEST_CASE(StatementWhileLoopBasic) { test_parse_string("int f () {\nwhile (true) {\n}\n}\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementWhileUnclosed) {test_parse_string("int f () {\nwhile (true) {\n", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(StatementWhileNoCondition) { test_parse_string("int f () {\nwhile () {\n}\n }\n", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(StatementWhileLoopComplexCondition) {test_parse_string("int f () {\nwhile (x > 0 & done) {int x = 0;}\n}\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementWhileLoopEmptyBody) {test_parse_string("int f () {\nwhile (true) ; }\n", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(StatementWhileLoopWithoutParenthesis) {test_parse_string("int f () {\nwhile x > 0 {} }\n", TestingOutcome::SUCCESS); }

BOOST_AUTO_TEST_CASE(WhileTest) {test_parse_string("int f () {\nwhilex > 0 {} }\n", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(WhileTest2) {test_parse_string("int f () {\n10;while x > 0 {} }\n", TestingOutcome::SUCCESS); }




BOOST_AUTO_TEST_CASE(StatementExpressionInt) {test_parse_string("int f () {\n55;\n}\n", TestingOutcome::SUCCESS); } 
BOOST_AUTO_TEST_CASE(StatementExpressionBool) {test_parse_string("int f () {\ntrue;\n}\n", TestingOutcome::SUCCESS); } 
BOOST_AUTO_TEST_CASE(StatementInvalidExpression) {test_parse_string("int f () {\nx + * 10;\n}\n", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(StatementVarAssignComplexExpr) {test_parse_string("int f () {\nx = y * 5 + 10;\n}\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementSequence) {test_parse_string("int f () {\nx = 10;\ny = x + 5;\n}\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementVarAssignNoSemicolon) {test_parse_string("int f () {\nx = 10 }\n", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(StatementExprInvalidToken) {test_parse_string("int f () {\n10 * / 5; }\n", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(StatementExpressionBinOp) {test_parse_string("int f () {\nx + 5;\n}\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementPrintSimple) {test_parse_string("int f () {\nprint(10); }\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementPrintSimple2) {test_parse_string("int f () {\nprint (10); }\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementPrintComplex) {test_parse_string("int f () {\nprint( (10 * 2) - 10); }\n", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(StatementPrintEmpty) {test_parse_string("int f () {\nprint(); }\n", TestingOutcome::PARSE_FAILED); }

// return  
BOOST_AUTO_TEST_CASE(ReturnSimpleTrue) {test_parse_string("int f () {return 2; }", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(ReturnSimpleFalse) {test_parse_string("int f () {return 2 }", TestingOutcome::PARSE_FAILED); }
BOOST_AUTO_TEST_CASE(ReturnBinopTrue) {test_parse_string("int f () {return 2 + 2; }", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(ReturnMultiLinesTrue) {test_parse_string("int f () { int x = 2; x = 4; return 2 + 2; }", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(ReturnVarTrue) {test_parse_string("int f () { int x = 2; x = 4; return x; }", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(ReturnWithSpace) {test_parse_string("int f () { int x = 2; x = 4; return(x); }", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(ReturnWithoutSpacePlusPar) {test_parse_string("int f () { int x = 2; x = 4; return (x); }", TestingOutcome::SUCCESS); }
BOOST_AUTO_TEST_CASE(ReturnWithoutSpaceAndPar) {test_parse_string("int f () { int x = 2; x = 4; return x; }", TestingOutcome::SUCCESS); }


//  "if", "else", "while", "return", "print"
// Using reserved keywords for funcdecl
BOOST_AUTO_TEST_CASE(ReservedIfFuncFailF) {test_parse_string("int if() { return 0;}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ReservedElseFuncFailF) {test_parse_string("int else() { return 0;}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ReservedWhileFuncFailF) {test_parse_string("int while() { return 0;}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ReservedReturnFuncFailF) {test_parse_string("int return() { return 0;}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ReservedPrintFuncFailF) {test_parse_string("int print() { return 0;}", TestingOutcome::PARSE_FAILED);}

// Using reserved keywords for vardecl
BOOST_AUTO_TEST_CASE(ReservedIfVarFailF) {test_parse_string("int if = 3;", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ReservedElseVarFailF) {test_parse_string("int else = 3;", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ReservedWhileVarFailF) {test_parse_string("int while = 3;", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ReservedReturnVarFailF) {test_parse_string("int return = 3;", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ReservedPrintVarFailF) {test_parse_string("int print = 3;", TestingOutcome::PARSE_FAILED);}

// Test using primitive types as variable names
BOOST_AUTO_TEST_CASE(PrimitiveTypeVarFailF) {test_parse_string("int int = 3;", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(PrimitiveTypeVarFailF2) {test_parse_string("bool bool = true;", TestingOutcome::PARSE_FAILED);}


// Test if else 
BOOST_AUTO_TEST_CASE(SimpleIf) {test_parse_string("int f() {if(true) { }}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfFail) {test_parse_string("int f() { ifelse(true) { }}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(SimpleIfFail2) {test_parse_string("int f() { else(true) { }}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(SimpleIfFail3) {test_parse_string("int f() { iff(true) { }}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(SimpleIfFail33) {test_parse_string("int f() { iif(true) { }}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(SimpleIfFail4) {test_parse_string("int f() { elseif(true) { }}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(SimpleIfFail5) {test_parse_string("int f() { ifif(true) { }}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(SimpleIfFail6) {test_parse_string("int f() { if(true){} elseif(true){ }}", TestingOutcome::PARSE_FAILED);}

BOOST_AUTO_TEST_CASE(SimpleIfWithBlock) {test_parse_string("int f() { if(true) { int x = 2; }}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfElse) {test_parse_string("int f() {if(true) { } else { }}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfElse2) {test_parse_string("int f() {if(true){ }else{ }}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfElseWithBlocks) {test_parse_string("int f() {if(true) { int x = 2; } else { int y = 2; }}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfElseElse) {test_parse_string("int f() { if(false) {  } else if(true) {} else {} }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleIfElseElseWithBlocks) {test_parse_string("int f() { if(false) { int x = 2; } else if(true) { int y = 2;} else { int z = 2;}}", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ComplexIfElseFile) {test_parse_file("../tests/parserTests/compIfElse.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(DeepNestIfElse) {test_parse_file("../tests/parserTests/deepNestIfElse.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ComplexIfElseFile2) {test_parse_file("../tests/parserTests/compIfElseElseElse.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SyntaxErrorInCondition) {test_parse_string("int f() { if(true false) { }}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(MissingParentheses) {test_parse_string("int f() { if true) { }}", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(IfWithVariableExpressions) {test_parse_string("int f() { int x = 5; if(x > 2) { }}", TestingOutcome::SUCCESS);}

// Arrays
BOOST_AUTO_TEST_CASE(SimpleArray) {test_parse_string("int[1] x = new int[5];", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleArray2) {test_parse_string("int[] x = new int[5];", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(twoDimArray) {test_parse_string("int[2] x = new int[5,2];", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(threeDimArray) {test_parse_string("int[3] x = new int[5,2,2];", TestingOutcome::SUCCESS);}


BOOST_AUTO_TEST_CASE(BigDim) {test_parse_string("int[10] x = new int[1,2,3,4,5,6,7,8,9,10];", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayIndex) {test_parse_string("int[1] a = new int[5]; int y = a[2];", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayIndexDim2) {test_parse_string("int[2] a = new int[5,5]; int y = a[2,3];", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayIndexDim3) {test_parse_string("int[3] a = new int[5,5]; int y = a[2,3,5];", TestingOutcome::SUCCESS);}



BOOST_AUTO_TEST_CASE(ArrayGiveValueOutsideOfFunction) {test_parse_string("int[1] a = new int[2]; a[1] = 4;", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ArrayGiveValue) {test_parse_string("int f() { int[1] a = new int[2]; a[1] = 4; }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayGiveValue2d) {test_parse_string(" int f() { int[2] a = new int[2,2]; a[1,2] = 4; }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayGiveValue3d) {test_parse_string("int f() { int[3] a = new int[2,2,4]; a[1,2,3] = 4; }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrayTypeBool) {test_parse_string("int f() { new bool[2,2,4]; a[1,2,3] = true; }", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(DimAsVar) {test_parse_string("int f() { int x = 2; int[x] a = new int[2,2]; }", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(DimAsFunc) {test_parse_string("int g { int f() { return 2; } int[f()] a = new int[2,2]; }", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ArrIndexFunc) {test_parse_string("int f() { return 1; } int[2] a = new int[2, 2]; int x = a[f(), f()]; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrIndexVar) {test_parse_string("int i = 1; int[2] a = new int[2, 2]; int x = a[i,i]; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrIndexVarWrong) {test_parse_string("int i = 1; int[2] a = new int[2, 2]; int x = a[,i,i]; ", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ArrIndexVarWrong2) {test_parse_string("int i = 1; int[2] a = new int[2, 2]; int x = a[i,i,]; ", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ArrExpFunc) {test_parse_string("int f() { return 1; } int[2] a = new int[f(), f()]; ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ArrExpVar) {test_parse_string("int i = 1; int[2] a = new int[i, i]; ", TestingOutcome::SUCCESS);}


// Class 
BOOST_AUTO_TEST_CASE(SimpleClass) {test_parse_string("class T {  } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(ClassFail) {test_parse_string("class T { int x = 2;  } ", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(ClassFail2) {test_parse_string("class if { int x;  } ", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(SimpleClassWithEExp) {test_parse_string("class T { int x; int y;  } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(SimpleClassNested) {test_parse_string("class T { class E { } } ", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(SimpleClassFunction) {test_parse_string("class T { int f(){return 0;} } ", TestingOutcome::PARSE_FAILED);}
 

 // Obj instantiation
BOOST_AUTO_TEST_CASE(SimpleObjInst) {test_parse_string("class T { } T a = new T();  ", TestingOutcome::SUCCESS);}
 

 // test dot operator: 
// BOOST_AUTO_TEST_CASE(DotOpSimple) {test_parse_string("int f() { a.x = 2; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(DotLongChain) {test_parse_string("int f() { int x = a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(DotFail) {test_parse_string("int f() { int x = a.b. } ", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(DotOpSimple2) {test_parse_string("int f() { int x = a.x; } ", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(DotMissingsemi) {test_parse_string("int f() { int x = a.x } ", TestingOutcome::PARSE_FAILED);}
BOOST_AUTO_TEST_CASE(UsingComma) {test_parse_string("int f() { int x = a.x,y; } ", TestingOutcome::PARSE_FAILED);}


// Testing files
BOOST_AUTO_TEST_CASE(simple_file) {test_parse_file("../tests/parserTests/simple.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(prog1) {test_parse_file("../tests/parserTests/prog1.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(prog2) {test_parse_file("../tests/parserTests/prog2.chad", TestingOutcome::SUCCESS);}

// Testing 
BOOST_AUTO_TEST_CASE(TestFuncCallBeforeFunc) {test_parse_file("../tests/parserTests/funcCall.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TestFuncCallAfterFunc) {test_parse_file("../tests/parserTests/funcCall2.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TestFuncCallComplex) {test_parse_file("../tests/parserTests/funcCallComplex.chad", TestingOutcome::SUCCESS);}
BOOST_AUTO_TEST_CASE(TestFuncCallWeird) {test_parse_file("../tests/parserTests/funcCallfuncParam.chad", TestingOutcome::SUCCESS);}
