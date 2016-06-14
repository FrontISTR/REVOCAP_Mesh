#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbBucket.h"
#include "Geometry/kmbBoxRegion.h"
#include "Geometry/kmbPoint3DContainerMArray.h"
#include "Test/Test_Common.h"

BOOST_AUTO_TEST_CASE(Volume_Simplex)
{
	kmb::Point3D a(0.0,0.0,0.0);
	kmb::Point3D b(1.0,0.0,0.0);
	kmb::Point3D c(0.0,1.0,0.0);
	kmb::Point3D d(drand(),drand(),drand());
	double v_abcd = kmb::Point3D::volume(a,b,c,d);
	BOOST_CHECK_CLOSE(v_abcd,d.z()/6.0,1.0e-10);
}

BOOST_AUTO_TEST_CASE(Volume_Tetra)
{
	kmb::Point3D a(drand(),drand(),drand());
	kmb::Point3D b(drand(),drand(),drand());
	kmb::Point3D c(drand(),drand(),drand());
	kmb::Point3D d(drand(),drand(),drand());
	double v_abcd = kmb::Point3D::volume(a,b,c,d);
	double v_dabc = kmb::Point3D::volume(d,a,b,c);
	BOOST_CHECK_CLOSE(v_abcd,-v_dabc,1.0e-10);
	kmb::Point3D x(drand(),drand(),drand());
	double v_abcx = kmb::Point3D::volume(a,b,c,x);
	double v_bcdx = kmb::Point3D::volume(b,c,d,x);
	double v_cdax = kmb::Point3D::volume(c,d,a,x);
	double v_dabx = kmb::Point3D::volume(d,a,b,x);
	BOOST_CHECK_CLOSE(v_abcd,v_abcx-v_bcdx+v_cdax-v_dabx,1.0e-10);
}

// 4点の法線ベクトル
BOOST_AUTO_TEST_CASE( NormalVector_of_Quadrilateral ){
	for(int count = 0;count<1000;++count){
		kmb::Point3D a0(drand(),drand(),drand());
		kmb::Point3D a1(drand(),drand(),drand());
		kmb::Point3D a2(drand(),drand(),drand());
		kmb::Point3D a3(drand(),drand(),drand());
		double a012 = kmb::Point3D::area(a0,a1,a2);
		double a230 = kmb::Point3D::area(a2,a3,a0);
		double a013 = kmb::Point3D::area(a0,a1,a3);
		double a123 = kmb::Point3D::area(a1,a2,a3);
		kmb::Vector3D n012 = kmb::Point3D::calcNormalVector(a0,a1,a2);
		kmb::Vector3D n230 = kmb::Point3D::calcNormalVector(a2,a3,a0);
		kmb::Vector3D n013 = kmb::Point3D::calcNormalVector(a0,a1,a3);
		kmb::Vector3D n123 = kmb::Point3D::calcNormalVector(a1,a2,a3);
		kmb::Vector3D n0123 = kmb::Point3D::calcNormalVector(a0,a1,a2,a3);
		kmb::Vector3D n012_230 = a012*n012+a230*n230;
		kmb::Vector3D n013_123 = a013*n013+a123*n123;
		n012_230.normalize();
		n013_123.normalize();
		BOOST_CHECK_CLOSE( n0123.x(), n012_230.x(), 1.0e-8);
		BOOST_CHECK_CLOSE( n0123.y(), n012_230.y(), 1.0e-8);
		BOOST_CHECK_CLOSE( n0123.z(), n012_230.z(), 1.0e-8);
		BOOST_CHECK_CLOSE( n0123.x(), n013_123.x(), 1.0e-8);
		BOOST_CHECK_CLOSE( n0123.y(), n013_123.y(), 1.0e-8);
		BOOST_CHECK_CLOSE( n0123.z(), n013_123.z(), 1.0e-8);
	}
}

// 4点の外心を求める
BOOST_AUTO_TEST_CASE( CircumCenter4 )
{
	for(int count = 0;count<1000;++count){
		kmb::Point3D a0(drand(),drand(),drand());
		kmb::Point3D a1(drand(),drand(),drand());
		kmb::Point3D a2(drand(),drand(),drand());
		kmb::Point3D a3(drand(),drand(),drand());
		kmb::Point3D center = kmb::Point3D::getCircumCenter(a0,a1,a2,a3);
		double r0 = kmb::Point3D::distanceSq(center,a0);
		double r1 = kmb::Point3D::distanceSq(center,a1);
		double r2 = kmb::Point3D::distanceSq(center,a2);
		double r3 = kmb::Point3D::distanceSq(center,a3);
		BOOST_CHECK_CLOSE( r0, r1, 1.0e-8 );
		BOOST_CHECK_CLOSE( r0, r2, 1.0e-8 );
		BOOST_CHECK_CLOSE( r0, r3, 1.0e-8 );
	}
}
BOOST_AUTO_TEST_CASE( Point_Bucket_Index )
{
	kmb::Point3DContainerMArray points;
	points.initialize(2000);
	for(int i=0;i<1000;i++){
		points.addPoint( drand(), drand(), drand() );
	}
	// 同じ座標に格納する
	for(int i=0;i<1000;i++){
		kmb::Point3D pt;
		points.getPoint(i,pt);
		points.addPoint( pt.x(), pt.y(), pt.z() );
	}
	kmb::Bucket3DGrid bucket;
	kmb::BoundingBox bbox = points.getBoundingBox();
	bucket.setRegionGrid( bbox, 100 );
	bucket.setContainer( &points );
	bucket.appendAll();
	for(kmb::nodeIdType i=0;i<1000;i++){
		kmb::Point3D pt;
		points.getPoint(i,pt);
		int index = bucket.getIndex( pt.x(), pt.y(), pt.z() );
		BOOST_CHECK( index >= 0 );
	}
}
