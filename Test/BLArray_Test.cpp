#define BOOST_TEST_MODULE BLArrayTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Test/Test_Common.h"
#include "Common/kmbBLArray.h"

BOOST_AUTO_TEST_SUITE( BLArray )

BOOST_AUTO_TEST_CASE( BLArray_Set )
{
	// double 5 個をひとまとまりとしてアクセスする
	kmb::BLArray<double,5> vArray0(DBL_MAX);
	kmb::BLArray<double,5> vArray1(DBL_MAX);
	kmb::BLArrayIndex aIndex0;
	kmb::BLArrayIndex aIndex1;
	size_t count = 1000u;
	vArray0.initialize(count);
	vArray1.initialize(count);

	BOOST_CHECK_EQUAL( aIndex0.getIndex(), 0u );
	BOOST_CHECK( vArray0.getSize() >= count );
	BOOST_CHECK( vArray0.getTopSize() > 0u );
	BOOST_CHECK( vArray0.getSubSize() > 0u );

	double v[5];
	double u[5];

	// BLArrayIndex を使った代入
	aIndex0 = vArray0.getBLArrayIndex(0);
	for(size_t i=0;i<count;++i){
		v[0] = static_cast<double>(i+0.0);
		v[1] = static_cast<double>(i+0.1);
		v[2] = static_cast<double>(i+0.2);
		v[3] = static_cast<double>(i+0.3);
		v[4] = static_cast<double>(i+0.4);
		vArray0.set(aIndex0,v);
		++aIndex0;
	}

	// 取得
	aIndex0 = vArray0.getBLArrayIndex(0);
	for(size_t i=0;i<count;++i){
		vArray0.get(aIndex0,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), u[4] );

		vArray0.get(i,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), u[4] );

		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), vArray0(aIndex0,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), vArray0(aIndex0,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), vArray0(aIndex0,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), vArray0(aIndex0,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), vArray0(aIndex0,4) );

		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), vArray0(i,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), vArray0(i,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), vArray0(i,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), vArray0(i,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), vArray0(i,4) );
		++aIndex0;
	}

	// int を使った代入
	for(size_t i=0;i<count;++i){
		v[0] = static_cast<double>(i+1.0);
		v[1] = static_cast<double>(i+1.1);
		v[2] = static_cast<double>(i+1.2);
		v[3] = static_cast<double>(i+1.3);
		v[4] = static_cast<double>(i+1.4);
		vArray1.set(i,v);
	}

	// 取得
	aIndex1 = vArray1.getBLArrayIndex(0);
	for(size_t i=0;i<count;++i){
		vArray1.get(aIndex1,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.4), u[4] );

		vArray1.get(i,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.4), u[4] );

		BOOST_CHECK_EQUAL( static_cast<double>(i+1.0), vArray1(aIndex1,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.1), vArray1(aIndex1,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.2), vArray1(aIndex1,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.3), vArray1(aIndex1,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.4), vArray1(aIndex1,4) );

		BOOST_CHECK_EQUAL( static_cast<double>(i+1.0), vArray1(i,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.1), vArray1(i,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.2), vArray1(i,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.3), vArray1(i,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.4), vArray1(i,4) );
		++aIndex1;
	}

	// inverse int を使った代入
	for(size_t i=0;i<count;++i){
		size_t i0 = count-1-i;
		v[0] = static_cast<double>(i0+2.0);
		v[1] = static_cast<double>(i0+2.1);
		v[2] = static_cast<double>(i0+2.2);
		v[3] = static_cast<double>(i0+2.3);
		v[4] = static_cast<double>(i0+2.4);
		vArray1.set(i0,v);
	}

	// 取得
	aIndex1 = vArray1.getBLArrayIndex(count-1);
	for(size_t i=0;i<count;++i){
		size_t i0 = count-1-i;
		vArray1.get(aIndex1,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.4), u[4] );

		vArray1.get(i0,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.4), u[4] );

		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.0), vArray1(aIndex1,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.1), vArray1(aIndex1,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.2), vArray1(aIndex1,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.3), vArray1(aIndex1,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.4), vArray1(aIndex1,4) );

		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.0), vArray1(i0,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.1), vArray1(i0,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.2), vArray1(i0,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.3), vArray1(i0,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.4), vArray1(i0,4) );
		--aIndex1;
	}

	// データの個数を 10 倍にする

	// BLArrayIndex を使った代入
	aIndex0 = vArray0.getBLArrayIndex(0);
	for(size_t i=0;i<count;++i){
		v[0] = static_cast<double>(i+0.0);
		v[1] = static_cast<double>(i+0.1);
		v[2] = static_cast<double>(i+0.2);
		v[3] = static_cast<double>(i+0.3);
		v[4] = static_cast<double>(i+0.4);
		vArray0.set(aIndex0,v);
		++aIndex0;
	}

	// 取得
	aIndex0 = vArray0.getBLArrayIndex(0);
	for(size_t i=0;i<count;++i){
		vArray0.get(aIndex0,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), u[4] );

		vArray0.get(i,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), u[4] );

		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), vArray0(aIndex0,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), vArray0(aIndex0,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), vArray0(aIndex0,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), vArray0(aIndex0,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), vArray0(aIndex0,4) );

		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), vArray0(i,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), vArray0(i,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), vArray0(i,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), vArray0(i,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), vArray0(i,4) );
		++aIndex0;
	}

	// int を使った代入
	for(size_t i=0;i<count;++i){
		v[0] = static_cast<double>(i+1.0);
		v[1] = static_cast<double>(i+1.1);
		v[2] = static_cast<double>(i+1.2);
		v[3] = static_cast<double>(i+1.3);
		v[4] = static_cast<double>(i+1.4);
		vArray1.set(i,v);
	}

	// 取得
	aIndex1 = vArray1.getBLArrayIndex(0);
	for(size_t i=0;i<count;++i){
		vArray1.get(aIndex1,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.4), u[4] );

		vArray1.get(i,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.4), u[4] );

		BOOST_CHECK_EQUAL( static_cast<double>(i+1.0), vArray1(aIndex1,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.1), vArray1(aIndex1,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.2), vArray1(aIndex1,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.3), vArray1(aIndex1,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.4), vArray1(aIndex1,4) );

		BOOST_CHECK_EQUAL( static_cast<double>(i+1.0), vArray1(i,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.1), vArray1(i,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.2), vArray1(i,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.3), vArray1(i,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i+1.4), vArray1(i,4) );
		++aIndex1;
	}

	// inverse int を使った代入
	for(size_t i=0;i<count;++i){
		size_t i0 = count-1-i;
		v[0] = static_cast<double>(i0+2.0);
		v[1] = static_cast<double>(i0+2.1);
		v[2] = static_cast<double>(i0+2.2);
		v[3] = static_cast<double>(i0+2.3);
		v[4] = static_cast<double>(i0+2.4);
		vArray1.set(i0,v);
	}

	// 取得
	aIndex1 = vArray1.getBLArrayIndex(count-1);
	for(size_t i=0;i<count;++i){
		size_t i0 = count-1-i;
		vArray1.get(aIndex1,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.4), u[4] );

		vArray1.get(i0,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.4), u[4] );

		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.0), vArray1(aIndex1,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.1), vArray1(aIndex1,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.2), vArray1(aIndex1,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.3), vArray1(aIndex1,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.4), vArray1(aIndex1,4) );

		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.0), vArray1(i0,0) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.1), vArray1(i0,1) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.2), vArray1(i0,2) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.3), vArray1(i0,3) );
		BOOST_CHECK_EQUAL( static_cast<double>(i0+2.4), vArray1(i0,4) );
		--aIndex1;
	}
}

// index を 100 から始めた場合
BOOST_AUTO_TEST_CASE( BLArray_Set_Shift )
{
	kmb::BLArray<double,5> vArray0(DBL_MAX);
	kmb::BLArrayIndex aIndex0;
	size_t count = 1000u;
	vArray0.initialize(count);

	double v[5];
	double u[5];

	// int を使った代入
	for(size_t i=100;i<2*count;++i){
		v[0] = static_cast<double>(i+0.0);
		v[1] = static_cast<double>(i+0.1);
		v[2] = static_cast<double>(i+0.2);
		v[3] = static_cast<double>(i+0.3);
		v[4] = static_cast<double>(i+0.4);
		vArray0.set(i,v);
	}

	// int を使った取得
	for(size_t i=100;i<2*count;++i){
		vArray0.get(i,u);
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.0), u[0] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.1), u[1] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.2), u[2] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.3), u[3] );
		BOOST_CHECK_EQUAL( static_cast<double>(i+0.4), u[4] );
	}
}

BOOST_AUTO_TEST_SUITE_END()
