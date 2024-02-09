#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestingTests
#include <boost/test/unit_test.hpp>

// Just a simple test to make sure the testing framework works

BOOST_AUTO_TEST_CASE(AlwaysTrue)
{
    BOOST_CHECK(true);
}
