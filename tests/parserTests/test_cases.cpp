
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



// BOOST_AUTO_TEST_CASE(prog1) {test_parse_file("../tests/parserTests/prog1.chad", TestingOutcome::SUCCESS);}
// BOOST_AUTO_TEST_CASE(prog2) {test_parse_file("../tests/parserTests/prog2.chad", TestingOutcome::SUCCESS);}