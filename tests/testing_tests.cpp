#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestingTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(AlwaysTrue)
{
    BOOST_CHECK(true);
}
