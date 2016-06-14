#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "Common/kmbArrayUtil.h"
#include "Test/Test_Common.h"

BOOST_AUTO_TEST_CASE( Array_Swap )
{
	int a[10];
	{
		for(int i=0;i<10;++i){
			a[i] = i;
		}
		// 0 1 2 3 4 5 6 7 8 9
		kmb::ArrayUtil::arraySwap( a, 2, 2, 7, 2 );
		printf("%d %d %d %d %d %d %d %d %d %d\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		// 0 1 7 8 4 5 6 2 3 9
		int b[10] = {0,1,7,8,4,5,6,2,3,9};
		for(int i=0;i<10;++i){
			BOOST_CHECK_EQUAL( b[i], a[i] );
		}
	}
	{
		for(int i=0;i<10;++i){
			a[i] = i;
		}
		// 0 1 2 3 4 5 6 7 8 9
		kmb::ArrayUtil::arraySwap( a, 2, 1, 7, 3 );
		printf("%d %d %d %d %d %d %d %d %d %d\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		// 0 1 7 8 9 3 4 5 6 2
		int b[10] = {0,1,7,8,9,3,4,5,6,2};
		for(int i=0;i<10;++i){
			BOOST_CHECK_EQUAL( b[i], a[i] );
		}
	}
	{
		for(int i=0;i<10;++i){
			a[i] = i;
		}
		// 0 1 2 3 4 5 6 7 8 9
		kmb::ArrayUtil::arraySwap( a, 0, 4, 8, 1 );
		printf("%d %d %d %d %d %d %d %d %d %d\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		// 8 4 5 6 7 0 1 2 3 9
		int b[10] = {8,4,5,6,7,0,1,2,3,9};
		for(int i=0;i<10;++i){
			BOOST_CHECK_EQUAL( b[i], a[i] );
		}
	}
	{
		for(int i=0;i<10;++i){
			a[i] = i;
		}
		// 0 1 2 3 4 5 6 7 8 9
		kmb::ArrayUtil::arraySwap( a, 2, 0, 6, 2 );
		printf("%d %d %d %d %d %d %d %d %d %d\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		// 0 1 6 7 2 3 4 5 8 9
		int b[10] = {0,1,6,7,2,3,4,5,8,9};
		for(int i=0;i<10;++i){
			BOOST_CHECK_EQUAL( b[i], a[i] );
		}
	}
	{
		for(int i=0;i<10;++i){
			a[i] = i;
		}
		// 0 1 2 3 4 5 6 7 8 9
		kmb::ArrayUtil::arraySwap( a, 1, 5, 8, 0 );
		printf("%d %d %d %d %d %d %d %d %d %d\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9]);
		// 0 6 7 1 2 3 4 5 8 9
		int b[10] = {0,6,7,1,2,3,4,5,8,9};
		for(int i=0;i<10;++i){
			BOOST_CHECK_EQUAL( b[i], a[i] );
		}
	}
}
