#define BOOST_TEST_MODULE ArrayTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Common/kmbArrayUtil.h"
#include "Test/Test_Common.h"

BOOST_AUTO_TEST_SUITE(Array)

BOOST_AUTO_TEST_CASE(ArrayIndex)
{
	double a[] = { 1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0, 128.0, 256.0, 512.0, 1024.0};
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 0.0), -1);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 1.5), 0);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 3.0), 1);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 5.0), 2);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 10.0), 3);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 20.0), 4);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 50.0), 5);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 100.0), 6);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 200.0), 7);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 300.0), 8);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 800.0), 9);
	BOOST_CHECK_EQUAL(kmb::ArrayUtil::arrayIndex(11, a, 1100.0), 10);
}

BOOST_AUTO_TEST_SUITE_END()
