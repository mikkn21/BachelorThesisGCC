#ifndef SYMBOL_COLLECTION_TEST_LIB_HPP
#define SYMBOL_COLLECTION_TEST_LIB_HPP


#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/spirit/home/x3.hpp>

enum class TestingOutcome {
    SUCCESS,
    FAILED
};

void testSymbolCollectionFile(std::string input, TestingOutcome testing_outcome);
void testSymbolCollectionString(std::string input, TestingOutcome testing_outcome);

#endif // SYMBOL_COLLECTION_TEST_LIB_HPP
