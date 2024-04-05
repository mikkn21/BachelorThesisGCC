#ifndef TYPE_CHECK_TEST_LIB_HPP
#define TYPE_CHECK_TEST_LIB_HPP


#include <boost/test/unit_test.hpp>

enum class TestingOutcome {
    SUCCESS,
    FAILED
};

void testTypeCheckFile(std::string input, TestingOutcome testing_outcome);
void testTypeCheckString(std::string input, TestingOutcome testing_outcome);

#endif // TYPE_CHECK_TEST_LIB_HPP 
