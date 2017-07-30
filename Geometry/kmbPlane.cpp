/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Plane                                                   #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
# Software Name : RevocapMesh version 1.2                              #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2008/03/14     #
#                                           T. Takeda   2008/03/14     #
#                                           K. Amemiya  2008/03/14     #
#                                                                      #
#      Contact Address: RSS21 Project, IIS, The University of Tokyo    #
#                                                                      #
#      "Innovative General-Purpose Coupled Analysis System"            #
#                                                                      #
----------------------------------------------------------------------*/
#include "Geometry/kmbPlane.h"
#include "Common/kmbTolerance.h"
#include <cmath>

kmb::Plane::Plane(const double _a,const double _b,const double _c,const double _d)
: a(_a)
, b(_b)
, c(_c)
, d(_d)
{
	double r = sqrt(a*a+b*b+c*c);
	if( r != 0.0 ){
		this->a = a/r;
		this->b = b/r;
		this->c = c/r;
		this->d = d/r;
	}
}

kmb::Plane::Plane(const kmb::Point3D &base,const kmb::Vector3D &normal)
	: a(normal.x()), b(normal.y()), c(normal.z()), d(-base.x()*normal.x()-base.y()*normal.y()-base.z()*normal.z())
{
	double r = sqrt(a*a+b*b+c*c);
	if( r != 0.0 ){
		this->a = a/r;
		this->b = b/r;
		this->c = c/r;
		this->d = d/r;
	}
}

kmb::Plane::~Plane(void)
{
}

kmb::Plane* kmb::Plane::
createFromPoints(const kmb::Point3D &p,const kmb::Point3D &q,const kmb::Point3D &r)
{
	kmb::Vector3D pq(q,p);
	kmb::Vector3D pr(r,p);
	kmb::Vector3D normal = pq%pr;  // 外積
	return kmb::Plane::createFromBaseNormal(p,normal);
}

kmb::Plane* kmb::Plane::
createFromBaseNormal(const kmb::Point3D &base,const kmb::Vector3D &normal)
{
	kmb::Plane* plane = NULL;
	double a = normal.x();
	double b = normal.y();
	double c = normal.z();
	double d = - (a*base.x() + b*base.y() + c*base.z());
	plane = new kmb::Plane(a,b,c,d);
	return plane;
}

kmb::Vector3D
kmb::Plane::calcNormal(const Point3D &a,const Point3D &b,const Point3D &c)
{
	kmb::Vector3D ab(b,a);
	kmb::Vector3D ac(c,a);
	kmb::Vector3D normal = ab%ac; // 外積
	normal.normalize();
	return normal;
}

double kmb::Plane::
evaluate(const kmb::Point3D &pt) const
{
	return this->evaluate(pt.x(),pt.y(),pt.z());
}

double kmb::Plane::
evaluate(const double x,const double y,const double z) const
{
	return a*x + b*y + c*z + d;
}

kmb::Vector3D
kmb::Plane::getNormal(void) const
{
	return kmb::Vector3D(a,b,c);
}

double
kmb::Plane::distance(const kmb::Point3D &pt) const
{
	return fabs(this->evaluate(pt));
}

double
kmb::Plane::distance(const double x,const double y,const double z) const
{
	return fabs(this->evaluate(x,y,z));
}

double
kmb::Plane::getConstant(void) const
{
	return d;
}

bool kmb::Plane::setNormal(double a,double b,double c)
{
	double r = sqrt(a*a+b*b+c*c);
	if( r != 0.0 ){
		this->a = a/r;
		this->b = b/r;
		this->c = c/r;
		return true;
	}
	return false;
}

void kmb::Plane::setOrigin(double x,double y,double z)
{
	d = -a*x-b*y-c*z;
}

kmb::Point3D* kmb::Plane::createIntersectPoint(const kmb::Point3D &p0, const kmb::Point3D &p1) const
{
	kmb::Point3D* point = NULL;
	double t0 = this->evaluate( p0 );
	double t1 = this->evaluate( p1 );
	if( t0 * t1 < 0 ){
		point = new kmb::Point3D();
		kmb::Point3D::dividingPoint(p0, p1, t0, -t1, *point);
	}
	return point;
}

bool kmb::Plane::getIntersection(const kmb::Point3D &p0, const kmb::Point3D &p1, kmb::Point3D &pt, double &t) const
{
	double t0 = this->evaluate( p0 );
	double t1 = this->evaluate( p1 );
	if( t0 != t1 ){
		kmb::Point3D::dividingPoint(p0, p1, t0, -t1, pt);
		t = t0 / (t0-t1);
		return true;
	}
	return false;
}

enum kmb::Plane::crossingTriangle kmb::Plane::getIntersectionTriangle
(const kmb::Point3D &p0,const kmb::Point3D &p1, const kmb::Point3D &p2, kmb::Point3D &p3, kmb::Point3D &p4) const
{
	double t0 = this->evaluate( p0 );
	double t1 = this->evaluate( p1 );
	double t2 = this->evaluate( p2 );
	if( t0 * t1 < 0.0 && t0 * t2 < 0.0 ){
		kmb::Point3D::dividingPoint(p0, p1, t0, -t1, p3);
		kmb::Point3D::dividingPoint(p0, p2, t0, -t2, p4);
		return (t0 > 0.0) ? k034_1243 : k1243_034;
	}else if( t1 * t2 < 0.0 && t1 * t0 < 0.0 ){
		kmb::Point3D::dividingPoint(p1, p2, t1, -t2, p3);
		kmb::Point3D::dividingPoint(p1, p0, t1, -t0, p4);
		return (t1 > 0.0) ? k134_2043 : k2043_134;
	}else if( t2 * t0 < 0.0 && t2 * t1 < 0.0 ){
		kmb::Point3D::dividingPoint(p2, p0, t2, -t0, p3);
		kmb::Point3D::dividingPoint(p2, p1, t2, -t1, p4);
		return (t2 > 0.0) ? k234_0143 : k0143_234;
	}else if( t1 * t2 < 0.0 ){
		// t0 * t1 >= 0.0 かつ t0 * t2 >= 0.0
		// p0 を通るとみなす
		kmb::Point3D::dividingPoint(p1, p2, t1, -t2, p3);
		return (t1 > 0.0) ? k0_013_032 : k0_032_013;
	}else if( t2 * t0 < 0.0 ){
		// t1 * t2 >= 0.0 かつ t1 * t0 >= 0.0
		// p1 を通るとみなす
		kmb::Point3D::dividingPoint(p2, p0, t2, -t0, p3);
		return (t2 > 0.0) ? k1_123_130 : k1_130_123;
	}else if( t0 * t1 < 0.0 ){
		// t2 * t0 >= 0.0 かつ t2 * t1 >= 0.0
		// p2 を通るとみなす
		kmb::Point3D::dividingPoint(p0, p1, t0, -t1, p3);
		return (t0 > 0.0) ? k2_203_231 : k2_231_203;
	}
	return (t0 > 0.0) ? kALL_POSITIVE : kALL_NEGATIVE;
}

kmb::Point3D
kmb::Plane::projectOnPlane(const kmb::Point3D& p) const
{
	double t = this->evaluate(p);
	return kmb::Point3D(p.x() - t*a,p.y()-t*b,p.z()-t*c);
}

void
kmb::PlaneXY::setZ(double z0)
{
	this->d = -z0;
}

void
kmb::PlaneYZ::setX(double x0)
{
	this->d = -x0;
}

void kmb::PlaneZX::setY(double y0)
{
	this->d = -y0;
}

// local function
double intersectXY(double zz, const kmb::Point3D &p0, const kmb::Point3D &p1, double z0, double z1, kmb::Point3D &pt)
{
	double t1 = 1.0 / (z1 - z0);
	double t0 = -z0 * t1;
	t1 =  z1 * t1;
	pt.setCoordinate(
		t1*p0.x() + t0*p1.x(),
		t1*p0.y() + t0*p1.y(),
		zz);
	return t0;
}

bool kmb::PlaneXY::getIntersection(const double z0,const kmb::Point3D &p0,const kmb::Point3D &p1, kmb::Point3D &pt, double &t)
{
	double diffZ = p1.z() - p0.z();
	if( diffZ != 0.0 ){
		t = intersectXY(z0,p0,p1,p0.z()-z0,p1.z()-z0,pt);
		return true;
	}
	return false;
}

double intersectYZ(double xx, const kmb::Point3D &p0, const kmb::Point3D &p1, double x0, double x1, kmb::Point3D &pt)
{
	double t1 = 1.0 / (x1 - x0);
	double t0 = -x0 * t1;
	t1 = x1 * t1;
	pt.setCoordinate(
		xx,
		t1*p0.y() + t0*p1.y(),
		t1*p0.z() + t0*p1.z()
	);
	return t0;
}

bool kmb::PlaneYZ::getIntersection(const double x0,const kmb::Point3D &p0,const kmb::Point3D &p1, kmb::Point3D &pt, double &t)
{
	double diffX = p1.x() - p0.x();
	if( diffX != 0.0 ){
		t = intersectYZ(x0, p0, p1, p0.x() - x0, p1.x() - x0, pt);
		return true;
	}
	return false;
}

double intersectZX(double yy, const kmb::Point3D &p0, const kmb::Point3D &p1, double y0, double y1, kmb::Point3D &pt)
{
	double t1 = 1.0 / (y1 - y0);
	double t0 = -y0 * t1;
	t1 = y1 * t1;
	pt.setCoordinate(
		t1*p0.x() + t0*p1.x(),
		yy,
		t1*p0.z() + t0*p1.z()
	);
	return t0;
}

bool kmb::PlaneZX::getIntersection(const double y0,const kmb::Point3D &p0,const kmb::Point3D &p1, kmb::Point3D &pt, double &t)
{
	double diffY = p1.y() - p0.y();
	if( diffY != 0.0 ){
		t = intersectZX(y0, p0, p1, p0.y() - y0, p1.y() - y0, pt);
		return true;
	}
	return false;
}

enum kmb::Plane::crossingTriangle kmb::PlaneXY::getIntersectionTriangle(
		const double zz,const kmb::Point3D &p0,const kmb::Point3D &p1, const kmb::Point3D &p2,kmb::Point3D &p3, kmb::Point3D &p4)
{
	double z0 = p0.z() - zz;
	double z1 = p1.z() - zz;
	double z2 = p2.z() - zz;
	if( z0 * z1 < 0.0 && z0 * z2 < 0.0 ){
		intersectXY(zz, p0, p1, z0, z1, p3);
		intersectXY(zz, p0, p2, z0, z2, p4);
		return (z0 > 0.0) ? k034_1243 : k1243_034;
	}else if( z1 * z2 < 0.0 && z1 * z0 < 0.0 ){
		intersectXY(zz, p1, p2, z1, z2, p3);
		intersectXY(zz, p1, p0, z1, z0, p4);
		return (z1 > 0.0) ? k134_2043 : k2043_134;
	}else if( z2 * z0 < 0.0 && z2 * z1 < 0.0 ){
		intersectXY(zz, p2, p0, z2, z0, p3);
		intersectXY(zz, p2, p1, z2, z1, p4);
		return (z2 > 0.0) ? k234_0143 : k0143_234;
	}else if( z1 * z2 < 0.0 && fabs(z0) < kmb::Tolerance::geometric ){
		// p0 を通るとみなす
		intersectXY(zz, p1, p2, z1, z2, p3);
		return (z1 > 0.0) ? k0_013_032 : k0_032_013;
	}else if( z2 * z0 < 0.0 && fabs(z1) < kmb::Tolerance::geometric ){
		// p1 を通るとみなす
		intersectXY(zz, p2, p0, z2, z0, p3);
		return (z2 > 0.0) ? k1_123_130 : k1_130_123;
	}else if( z0 * z1 < 0.0 && fabs(z2) < kmb::Tolerance::geometric ){
		// p2 を通るとみなす
		intersectXY(zz, p0, p1, z0, z1, p3);
		return (z0 > 0.0) ? k2_203_231 : k2_231_203;
	}
	return (z0+z1+z2 > 0.0) ? kALL_POSITIVE : kALL_NEGATIVE;
}

enum kmb::Plane::crossingTriangle kmb::PlaneYZ::getIntersectionTriangle(
		const double xx,const kmb::Point3D &p0,const kmb::Point3D &p1, const kmb::Point3D &p2,kmb::Point3D &p3, kmb::Point3D &p4)
{
	double x0 = p0.x() - xx;
	double x1 = p1.x() - xx;
	double x2 = p2.x() - xx;
	if( x0 * x1 < 0.0 && x0 * x2 < 0.0 ){
		intersectYZ(xx, p0, p1, x0, x1, p3);
		intersectYZ(xx, p0, p2, x0, x2, p4);
		return (x0 > 0.0) ? k034_1243 : k1243_034;
	}else if( x1 * x2 < 0.0 && x1 * x0 < 0.0 ){
		intersectYZ(xx, p1, p2, x1, x2, p3);
		intersectYZ(xx, p1, p0, x1, x0, p4);
		return (x1 > 0.0) ? k134_2043 : k2043_134;
	}else if( x2 * x0 < 0.0 && x2 * x1 < 0.0 ){
		intersectYZ(xx, p2, p0, x2, x0, p3);
		intersectYZ(xx, p2, p1, x2, x1, p4);
		return (x2 > 0.0) ? k234_0143 : k0143_234;
	}else if( x1 * x2 < 0.0 && fabs(x0) < kmb::Tolerance::geometric ){
		// p0 を通るとみなす
		intersectYZ(xx, p1, p2, x1, x2, p3);
		return (x1 > 0.0) ? k0_013_032 : k0_032_013;
	}else if( x2 * x0 < 0.0 && fabs(x1) < kmb::Tolerance::geometric ){
		// p1 を通るとみなす
		intersectYZ(xx, p2, p0, x2, x0, p3);
		return (x2 > 0.0) ? k1_123_130 : k1_130_123;
	}else if( x0 * x1 < 0.0 && fabs(x2) < kmb::Tolerance::geometric ){
		// p2 を通るとみなす
		intersectYZ(xx, p0, p1, x0, x1, p3);
		return (x0 > 0.0) ? k2_203_231 : k2_231_203;
	}
	return (x0+x1+x2 > 0.0) ? kALL_POSITIVE : kALL_NEGATIVE;
}

enum kmb::Plane::crossingTriangle kmb::PlaneZX::getIntersectionTriangle(
		const double yy,const kmb::Point3D &p0,const kmb::Point3D &p1, const kmb::Point3D &p2,kmb::Point3D &p3, kmb::Point3D &p4)
{
	double y0 = p0.y() - yy;
	double y1 = p1.y() - yy;
	double y2 = p2.y() - yy;
	if( y0 * y1 < 0.0 && y0 * y2 < 0.0 ){
		intersectZX(yy, p0, p1, y0, y1, p3);
		intersectZX(yy, p0, p2, y0, y2, p4);
		return (y0 > 0.0) ? k034_1243 : k1243_034;
	}else if( y1 * y2 < 0.0 && y1 * y0 < 0.0 ){
		intersectZX(yy, p1, p2, y1, y2, p3);
		intersectZX(yy, p1, p0, y1, y0, p4);
		return (y1 > 0.0) ? k134_2043 : k2043_134;
	}else if( y2 * y0 < 0.0 && y2 * y1 < 0.0 ){
		intersectZX(yy, p2, p0, y2, y0, p3);
		intersectZX(yy, p2, p1, y2, y1, p4);
		return (y2 > 0.0) ?k234_0143 : k0143_234;
	}else if( y1 * y2 < 0.0 && fabs(y0) < kmb::Tolerance::geometric ){
		// p0 を通るとみなす
		intersectZX(yy, p1, p2, y1, y2, p3);
		return (y1 > 0.0) ? k0_013_032 : k0_032_013;
	}else if( y2 * y0 < 0.0 && fabs(y1) < kmb::Tolerance::geometric ){
		// p1 を通るとみなす
		intersectZX(yy, p2, p0, y2, y0, p3);
		return (y2 > 0.0) ? k1_123_130 : k1_130_123;
	}else if( y0 * y1 < 0.0 && fabs(y2) < kmb::Tolerance::geometric ){
		// p2 を通るとみなす
		intersectZX(yy, p0, p1, y0, y1, p3);
		return (y0 > 0.0) ? k2_203_231 : k2_231_203;
	}
	return (y0+y1+y2 > 0.0) ? kALL_POSITIVE : kALL_NEGATIVE;
}
