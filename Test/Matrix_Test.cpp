#define BOOST_TEST_MODULE MatrixTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Matrix/kmbMatrix.h"
#include "Matrix/kmbMatrix_DoubleArray.h"
#include "Test/Test_Common.h"

BOOST_AUTO_TEST_SUITE(Matrix)

BOOST_AUTO_TEST_CASE(Determinant)
{
	kmb::SquareMatrix_DoubleArray mtx(3);
	mtx.set(0,0,1.0);
	mtx.set(0,1,1.0);
	mtx.set(0,2,1.0);
	mtx.set(1,0,2.0);
	mtx.set(1,1,3.0);
	mtx.set(1,2,5.0);
	mtx.set(2,0,4.0);
	mtx.set(2,1,9.0);
	mtx.set(2,2,25.0);
	BOOST_CHECK_CLOSE(mtx.determinant(),6.0,1.0e-10);
}

BOOST_AUTO_TEST_CASE(Solve_10x10)
{
	kmb::SquareMatrix_DoubleArray a(10);
	kmb::ColumnVector_DoubleArray b(10);
	kmb::ColumnVector_DoubleArray x(10);
	kmb::ColumnVector_DoubleArray b2(10);
	for(int i=0;i<10;++i){
		for(int j=0;j<10;++j){	
			a.set(i,j,drand());
		}
		b.setRow(i,drand());
	}
	a.solve(b,x);
	a.multiply_left(x,b2);
	for(int i=0;i<10;++i){
		BOOST_CHECK_CLOSE(b[i],b2[i],1.0e-10);
	}
}

BOOST_AUTO_TEST_SUITE_END()
