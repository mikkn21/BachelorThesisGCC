#define BOOST_TEST_MODULE AssemblyTests
#define BOOST_TEST_DYN_LINK  

#include "../lib/testing_assembly_lib.hpp"


BOOST_AUTO_TEST_CASE(Print) {test_assembly_file("test_print.chad", "1\n0\ntrue\nfalse\n9123923\n2\ntrue\nfalse");}
BOOST_AUTO_TEST_CASE(Binop) {test_assembly_file("testBinop.chad", "13\n7\n30\n3\n1\ntrue\nfalse\nfalse\ntrue\nfalse\ntrue\ntrue\nfalse\ntrue\nfalse");}
BOOST_AUTO_TEST_CASE(Precedence) {test_assembly_file("test_precedence.chad","4\n4\n49\n38\n0\n7\n18\n18\n12\n337\ntrue");}
BOOST_AUTO_TEST_CASE(FunctionCall) {test_assembly_file("test_function_call.chad","5");}
BOOST_AUTO_TEST_CASE(FunctionReturn) {test_assembly_file("test_function_return.chad","6");}
BOOST_AUTO_TEST_CASE(FunctionParamter) {test_assembly_file("test_function_parameters.chad","12");}
BOOST_AUTO_TEST_CASE(IfElse) {test_assembly_file("test_if_else.chad","2\n3\n5\n7");}
BOOST_AUTO_TEST_CASE(While) {test_assembly_file("test_while.chad","2\n3\n4\ntrue");}
BOOST_AUTO_TEST_CASE(NestedWhile) {test_assembly_file("nestedwhile.chad","0\n1\n2\n3");}
BOOST_AUTO_TEST_CASE(BreakContinue) {test_assembly_file("breakcont.chad","1\n3");}
BOOST_AUTO_TEST_CASE(DynamicLinking) {test_assembly_file("dynamic_linking.chad","3628800");}
BOOST_AUTO_TEST_CASE(StaticLinkingRead) {test_assembly_file("static_linking_read.chad","1\n1\n2\n5\n6\n7\n8\n9");}
BOOST_AUTO_TEST_CASE(StaticLinkingWrite) {test_assembly_file("static_linking_write.chad","0\n10\n20\n50\n11");}
BOOST_AUTO_TEST_CASE(Arrays) {test_assembly_file("test_arrays.chad","42\n17\n29\n8");}
BOOST_AUTO_TEST_CASE(ArraysInit) {test_assembly_file("test_array_init.chad","0\n0\n0\n0");}
BOOST_AUTO_TEST_CASE(BasicStructs) {test_assembly_file("basic_structs.chad","0\n3\n0\n0\n3\n0\n5");}

BOOST_AUTO_TEST_CASE(BetaArray) {test_assembly_string("int main(){ int[1] arr; if arr == beta {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaArrayRev) {test_assembly_string("int main(){ int[1] arr; if beta == arr {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaArray2) {test_assembly_string("int main(){ int[1] arr = new int[10]; if arr != beta {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaArray2Rev) {test_assembly_string("int main(){ int[1] arr = new int[10]; if beta != arr {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaClass) {test_assembly_string("class T{} int main(){ T t; if t == beta {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaClassRev) {test_assembly_string("class T{} int main(){ T t; if beta == t {print(0);} return 0; }", "0");}
BOOST_AUTO_TEST_CASE(BetaClass2) {test_assembly_string("class T{} int main(){ T t = new T(); if t != beta {print(0);} return 0; }", "0");}

BOOST_AUTO_TEST_CASE(PrintTrue) {test_assembly_string("int main(){print(true); return 0;}", "true");}
BOOST_AUTO_TEST_CASE(PrintFalse) {test_assembly_string("int main(){print(false); return 0;}", "false");}
BOOST_AUTO_TEST_CASE(PrintObject) {test_assembly_string("class T{} int main(){T t = new T(); print(t); return 0;}", "object");}
BOOST_AUTO_TEST_CASE(PrintObjectBeta) {test_assembly_string("class T{} int main(){T t = beta; print(t); return 0;}", "beta");}
BOOST_AUTO_TEST_CASE(PrintObjectBetaNoInit) {test_assembly_string("class T{} int main(){T t; print(t); return 0;}", "beta");}
BOOST_AUTO_TEST_CASE(PrintArray) {test_assembly_string("int main(){int[3] arr = new int[10,10,10]; print(arr); return 0;}", "array");}
BOOST_AUTO_TEST_CASE(PrintArrayBeta) {test_assembly_string("int main(){int[3] arr = beta; print(arr); return 0;}", "beta");}
BOOST_AUTO_TEST_CASE(PrintArrayBetaNoInit) {test_assembly_string("int main(){int[3] arr; print(arr); return 0;}", "beta");}
BOOST_AUTO_TEST_CASE(PrintBeta) {test_assembly_string("int main(){print(beta); return 0;}", "beta");}

BOOST_AUTO_TEST_CASE(BetaAccessArray) {test_assembly_string("int main(){int[3] arr; arr[2,4,5]; return 0;}", "Attempted to access a beta value on line 1");}
BOOST_AUTO_TEST_CASE(BetaAccessArrayExplicit) {test_assembly_string("int main(){int[3] arr = beta; arr[2,4,5]; return 0;}", "Attempted to access a beta value on line 1");}
BOOST_AUTO_TEST_CASE(BetaAccessArrayExp) {test_assembly_string("int main(){int[3] arr; int x = arr[2,3,4]; return 0;}", "Attempted to access a beta value on line 1");}
BOOST_AUTO_TEST_CASE(BetaAccessArrayLine2) {test_assembly_string("int main(){int[3] arr;\n arr[2,4,5]; return 0;}", "Attempted to access a beta value on line 2");}
BOOST_AUTO_TEST_CASE(BetaAccessArrayLine4) {test_assembly_string("int main(){int[3] arr;\n\n\n arr[2,4,5]; return 0;}", "Attempted to access a beta value on line 4");}


