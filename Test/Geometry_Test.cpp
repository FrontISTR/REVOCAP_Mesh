#define BOOST_TEST_MODULE GeometryTest

#ifdef BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#else
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#endif

#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbLine.h"
#include "Geometry/kmbBucket.h"
#include "Geometry/kmbBoxRegion.h"
#include "Geometry/kmbPoint3DContainerMArray.h"
#include "Test/Test_Common.h"

class Fixture{
public:
	Fixture(){
		irand();
	}
	~Fixture(){
	}
};

BOOST_GLOBAL_FIXTURE( Fixture );

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

BOOST_AUTO_TEST_CASE( PointToLine )
{
	int loopCount = 10;
	for(int k=0;k<loopCount;++k){
		double u = 0.0;
		kmb::Point3D x(drand(),drand(),drand());
		kmb::Point3D base(drand(),drand(),drand());
		kmb::Vector3D dir(drand(),drand(),drand());
		kmb::Line3D* line = kmb::Line3D::createFromBaseDirection(base,dir);
		double distSq = line->distanceSq(x);
		// -10.0 <= s <= 10.0
		int num = 200;
		for(int i=0;i<=num;++i){
			double s = -10.0 + 20.0 * i / static_cast<double>(num);
			kmb::Point3D p = base + s * dir;
			BOOST_CHECK( distSq <= p.distanceSq(x) + 1.0e-10 );
		}
		delete line;
	}
}

// 線分と点との距離
BOOST_AUTO_TEST_CASE( PointToSegment )
{
	int loopCount = 10;
	for(int k=0;k<loopCount;++k){
		double u = 0.0;
		kmb::Point3D a0(drand(),drand(),drand());
		kmb::Point3D a1(drand(),drand(),drand());
		kmb::Point3D q(drand(),drand(),drand());
		double distSq = q.distanceSqToSegment(a0,a1,u);
		// 線分上を 100 分割して、その点からの距離以下であることを確認する
		// 0.0 <= s <= 1.0
		int num = 100;
		for(int i=0;i<=num;++i){
			double s = i / static_cast<double>(num);
			BOOST_CHECK( 0.0 <= s );
			BOOST_CHECK( s <= 1.0 );
			kmb::Point3D p = a0.proportionalPoint(a1,s);
			BOOST_CHECK_CLOSE( p.x(), (1.0-s)*a0.x() + s*a1.x(), 1.0e-10 );
			BOOST_CHECK_CLOSE( p.y(), (1.0-s)*a0.y() + s*a1.y(), 1.0e-10 );
			BOOST_CHECK_CLOSE( p.z(), (1.0-s)*a0.z() + s*a1.z(), 1.0e-10 );
			BOOST_CHECK( distSq <= p.distanceSq(q) + 1.0e-10 );
		}
	}
}

BOOST_AUTO_TEST_CASE( PointToTriangle )
{
	int loopCount = 10;
	for(int k=0;k<loopCount;++k){
		double u0[3] = { 0.0, 0.0, 0.0 };
		kmb::Point3D a0(drand(),drand(),drand());
		kmb::Point3D a1(drand(),drand(),drand());
		kmb::Point3D a2(drand(),drand(),drand());
		kmb::Point3D q(drand(),drand(),drand());
		// 三角形 a0 a1 a2 と q との最短距離
		double distSq0 = q.distanceSqToTriangle(a0,a1,a2,u0);
		// 0.0 <= s <= 1.0
		// 0.0 <= t <= 1.0
		// s+t <= 1.0
		int num = 100;
		for(int i=0;i<=num;++i){
			for(int j=0;j<=num-i;++j){
				double s = i / static_cast<double>(num);
				double t = j / static_cast<double>(num);
				BOOST_CHECK( 0.0 <= s );
				BOOST_CHECK( 0.0 <= t );
				BOOST_CHECK( s + t <= 1.0 );
				kmb::Point3D p = a0 + s * kmb::Vector3D(a1,a0) + t * kmb::Vector3D(a2,a0);
				BOOST_CHECK_CLOSE( p.x(), (1.0-s-t)*a0.x() + s*a1.x() + t*a2.x(), 1.0e-10 );
				BOOST_CHECK_CLOSE( p.y(), (1.0-s-t)*a0.y() + s*a1.y() + t*a2.y(), 1.0e-10 );
				BOOST_CHECK_CLOSE( p.z(), (1.0-s-t)*a0.z() + s*a1.z() + t*a2.z(), 1.0e-10 );
				// 三角形 a0 a1 a2 上の点 p と q との距離
				BOOST_CHECK( distSq0 <= p.distanceSq(q) + 1.0e-10 );
			}
		}
	}
}
