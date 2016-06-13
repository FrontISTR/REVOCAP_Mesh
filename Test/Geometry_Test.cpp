#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "Geometry/kmbGeometry3D.h"
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
