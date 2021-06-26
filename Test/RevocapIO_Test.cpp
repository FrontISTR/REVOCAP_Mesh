#define BOOST_TEST_MODULE RevocapIOTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

BOOST_AUTO_TEST_SUITE(RevocapIO)

BOOST_AUTO_TEST_CASE(STLIO)
{
	int n=0;
	BOOST_CHECK_EQUAL(n,0);
}

BOOST_AUTO_TEST_SUITE_END()
