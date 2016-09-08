#define BOOST_TEST_MODULE CommonTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Common/kmbArrayUtil.h"
#include "Common/kmbPermutation.h"
#include "Common/kmbIndex.h"
#include "Test/Test_Common.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(Common)

BOOST_AUTO_TEST_CASE( Array3 )
{
	kmb::Array3<double> a(20,30,40);
	kmb::Array3<double,kmb::kReverseOrder> b(20,30,40);
	for(int i=0;i<20;i++){
		for(int j=0;j<30;j++){
			for(int k=0;k<40;k++){
				a.set(i,j,k, 10.0*i + 3.0*j - 2.0*k );
				b.set(i,j,k, 10.0*i + 3.0*j - 2.0*k );
			}
		}
	}
	for(int j=0;j<30;j++){
		for(int k=0;k<40;k++){
			for(int i=0;i<20;i++){
				double v;
				a.get(i,j,k,v);
				BOOST_CHECK_CLOSE( v, 10.0*i + 3.0*j - 2.0*k, 1.0e-20 );
				b.get(i,j,k,v);
				BOOST_CHECK_CLOSE( v, 10.0*i + 3.0*j - 2.0*k, 1.0e-20 );
			}
		}
	}
	int count = 0;
	kmb::Array3<double>::indexType end(20,30,40);
	for(kmb::Array3<double>::indexType ind(0,0,0);ind.valid(end);ind.next(end)){
		double v;
		a.get(ind,v);
		BOOST_CHECK_CLOSE( v, 10.0*ind.i + 3.0*ind.j - 2.0*ind.k, 1.0e-20 );
		++count;
	}
	BOOST_CHECK_EQUAL(count,20*30*40);
}

BOOST_AUTO_TEST_CASE( P_5_3 )
{
	kmb::Permutation perm;
	perm.initialize(5,3);
	int count = 0;
	while( !perm.isFinished() ){
		std::cout << count << " : " << perm.getPerm(0) << " " << perm.getPerm(1) << " " << perm.getPerm(2) << " " << std::endl;
		++count;
		++perm;
	}
	BOOST_CHECK_EQUAL(count,60);
}

BOOST_AUTO_TEST_CASE( P_5_3_2 )
{
	kmb::Permutation perm;
	perm.initialize(5,3);
	int count = 0;
	while( !perm.isFinished() ){
		std::cout << count << " : " << perm.getPerm(0) << " " << perm.getPerm(1) << " " << perm.getPerm(2) << " " << std::endl;
		++count;
		if( count % 2 == 0 ){
			perm.nextPerm(2);
		}else{
			perm.nextPerm(1);
		}
	}
	BOOST_CHECK_EQUAL(count,39);
}

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

BOOST_AUTO_TEST_SUITE_END()
