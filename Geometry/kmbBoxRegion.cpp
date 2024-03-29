/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : BoxRegion                                               #
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
#include "Geometry/kmbBoxRegion.h"
#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbPlane.h"
#include "Common/kmbCalculator.h"
#include "Geometry/kmbBoundingBox.h"

#include <cmath>
#include <cfloat>

double kmb::BoxRegion::thres = 1.0e-20;

kmb::BoxRegion::BoxRegion(void)
: maxPoint(0.0,0.0,0.0), minPoint(0.0,0.0,0.0)
{
}

kmb::BoxRegion::BoxRegion(const kmb::Point3D &l,const kmb::Point3D &u)
{
	setMinMax(l.x(), l.y(), l.z(), u.x(), u.y(), u.z());
}

kmb::BoxRegion::BoxRegion(double x0,double y0,double z0,double x1,double y1,double z1)
{
	setMinMax(x0,y0,z0,x1,y1,z1);
}

kmb::BoxRegion::BoxRegion(const kmb::BoxRegion &other)
: maxPoint(other.maxPoint)
, minPoint(other.minPoint)
{
}

kmb::BoxRegion::~BoxRegion(void)
{
}

kmb::BoxRegion&
kmb::BoxRegion::operator=(const kmb::BoxRegion &other)
{
	maxPoint = other.maxPoint;
	minPoint = other.minPoint;
	return *this;
}

void
kmb::BoxRegion::setMinMax(double x0,double y0,double z0,double x1,double y1,double z1)
{
	if(x0 < x1){
		this->minPoint.x( x0 );
		this->maxPoint.x( x1 );
	}else{
		this->minPoint.x( x1 );
		this->maxPoint.x( x0 );
	}

	if(y0 < y1){
		this->minPoint.y( y0 );
		this->maxPoint.y( y1 );
	}else{
		this->minPoint.y( y1 );
		this->maxPoint.y( y0 );
	}

	if(z0 < z1){
		this->minPoint.z( z0 );
		this->maxPoint.z( z1 );
	}else{
		this->minPoint.z( z1 );
		this->maxPoint.z( z0 );
	}
}

void kmb::BoxRegion::setMinMax(kmb::Point3D& minPoint,kmb::Point3D& maxPoint)
{
	setMinMax(minPoint.x(),minPoint.y(),minPoint.z(),maxPoint.x(),maxPoint.y(),maxPoint.z());
}

void
kmb::BoxRegion::getCenter(kmb::Point3D& center) const
{
	center.setCoordinate(
		( maxPoint.x() + minPoint.x() ) * 0.5,
		( maxPoint.y() + minPoint.y() ) * 0.5,
		( maxPoint.z() + minPoint.z() ) * 0.5 );
}

double
kmb::BoxRegion::minX(void) const
{
	return this->minPoint.x();
}

double
kmb::BoxRegion::minY(void) const
{
	return this->minPoint.y();
}

double
kmb::BoxRegion::minZ(void) const
{
	return this->minPoint.z();
}

double
kmb::BoxRegion::maxX(void) const
{
	return this->maxPoint.x();
}

double
kmb::BoxRegion::maxY(void) const
{
	return this->maxPoint.y();
}

double
kmb::BoxRegion::maxZ(void) const
{
	return this->maxPoint.z();
}

void kmb::BoxRegion::minX(double x)
{
	minPoint.x(x);
}

void kmb::BoxRegion::minY(double y)
{
	minPoint.y(y);
}

void kmb::BoxRegion::minZ(double z)
{
	minPoint.z(z);
}

void kmb::BoxRegion::maxX(double x)
{
	maxPoint.x(x);
}

void kmb::BoxRegion::maxY(double y)
{
	maxPoint.y(y);
}

void kmb::BoxRegion::maxZ(double z)
{
	maxPoint.z(z);
}

double
kmb::BoxRegion::centerX(void) const
{
	return 0.5*(this->maxPoint.x()+this->minPoint.x());
}

double
kmb::BoxRegion::centerY(void) const
{
	return 0.5*(this->maxPoint.y()+this->minPoint.y());
}

double
kmb::BoxRegion::centerZ(void) const
{
	return 0.5*(this->maxPoint.z()+this->minPoint.z());
}

double
kmb::BoxRegion::rangeX(void) const
{
	return this->maxPoint.x() - this->minPoint.x();
}

double
kmb::BoxRegion::rangeY(void) const
{
	return this->maxPoint.y() - this->minPoint.y();
}

double
kmb::BoxRegion::rangeZ(void) const
{
	return this->maxPoint.z() - this->minPoint.z();
}

double
kmb::BoxRegion::range(void) const
{
	return kmb::Maximizer::getMax( rangeX(), rangeY(), rangeZ() );
}

double
kmb::BoxRegion::diameter(void) const
{
	return maxPoint.distance( minPoint );
}

double
kmb::BoxRegion::diameterSq(void) const
{
	return maxPoint.distanceSq( minPoint );
}

const kmb::Point3D& kmb::BoxRegion::getMin(void) const
{
	return this->minPoint;
}

const kmb::Point3D& kmb::BoxRegion::getMax(void) const
{
	return this->maxPoint;
}

double
kmb::BoxRegion::getVolume(void) const
{
	return rangeX() * rangeY() * rangeZ();
}

kmb::Region::locationType
kmb::BoxRegion::intersect(const kmb::Point3D &point) const
{
	return intersect( point.x(), point.y(), point.z() );
}

kmb::Region::locationType
kmb::BoxRegion::intersect(const double x,const double y,const double z) const
{
	kmb::Region::locationType tx,ty,tz;
	if( this->minPoint.x() < x && x < this->maxPoint.x() ){
		tx = kmb::Region::INSIDE;
	}else if( this->minPoint.x() == x || x == this->maxPoint.x() ){
		tx = kmb::Region::ON_BOUNDARY;
	}else{
		tx = kmb::Region::OUTSIDE;
	}

	if( this->minPoint.y() < y && y < this->maxPoint.y() ){
		ty = kmb::Region::INSIDE;
	}else if( this->minPoint.y() == y || y == this->maxPoint.y() ){
		ty = kmb::Region::ON_BOUNDARY;
	}else{
		ty = kmb::Region::OUTSIDE;
	}

	if( this->minPoint.z() < z && z < this->maxPoint.z() ){
		tz = kmb::Region::INSIDE;
	}else if( this->minPoint.z() == z || z == this->maxPoint.z() ){
		tz = kmb::Region::ON_BOUNDARY;
	}else{
		tz = kmb::Region::OUTSIDE;
	}

	if( tx == kmb::Region::OUTSIDE ||
		ty == kmb::Region::OUTSIDE ||
		tz == kmb::Region::OUTSIDE )
	{
		return kmb::Region::OUTSIDE;
	}else if(
		tx == kmb::Region::ON_BOUNDARY ||
		ty == kmb::Region::ON_BOUNDARY ||
		tz == kmb::Region::ON_BOUNDARY )
	{
		return kmb::Region::ON_BOUNDARY;
	}else{
		return kmb::Region::INSIDE;
	}
}

double
kmb::BoxRegion::distanceSq(const kmb::Point3D& point) const
{
	double xdiff=0;
	double ydiff=0;
	double zdiff=0;

	if( point.x() < minPoint.x() ){
		xdiff = point.x() - minPoint.x();
	}
	if( point.x() < maxPoint.x() ){
		xdiff = 0.0;
	}else{
		xdiff = point.x() - maxPoint.x();
	}

	if( point.y() < minPoint.y() ){
		ydiff = point.y() - minPoint.y();
	}
	if( point.y() < maxPoint.y() ){
		ydiff = 0.0;
	}else{
		ydiff = point.y() - maxPoint.y();
	}

	if( point.z() < minPoint.z() ){
		zdiff = point.z() - minPoint.z();
	}
	if( point.z() < maxPoint.z() ){
		zdiff = 0.0;
	}else{
		zdiff = point.z() - maxPoint.z();
	}
	return xdiff*xdiff + ydiff*ydiff + zdiff*zdiff;
}

double
kmb::BoxRegion::distanceSq(const double x,const double y,const double z) const
{
	return this->distanceSq( kmb::Point3D(x,y,z) );
}

bool
kmb::BoxRegion::intersect(const kmb::BoxRegion &box) const
{
	double lowX = kmb::Maximizer::getMax( minX(), box.minX() );
	double lowY = kmb::Maximizer::getMax( minY(), box.minY() );
	double lowZ = kmb::Maximizer::getMax( minZ(), box.minZ() );
	double highX = kmb::Minimizer::getMin( maxX(), box.maxX() );
	double highY = kmb::Minimizer::getMin( maxY(), box.maxY() );
	double highZ = kmb::Minimizer::getMin( maxZ(), box.maxZ() );
	if( lowX < highX && lowY < highY && lowZ < highZ ){
		return true;
	}else{
		return false;
	}
}

double
kmb::BoxRegion::intersectVolume(const BoxRegion& box) const
{
	double lowX = kmb::Maximizer::getMax( minX(), box.minX() );
	double lowY = kmb::Maximizer::getMax( minY(), box.minY() );
	double lowZ = kmb::Maximizer::getMax( minZ(), box.minZ() );
	double highX = kmb::Minimizer::getMin( maxX(), box.maxX() );
	double highY = kmb::Minimizer::getMin( maxY(), box.maxY() );
	double highZ = kmb::Minimizer::getMin( maxZ(), box.maxZ() );
	if( lowX < highX && lowY < highY && lowZ < highZ ){
		return (highX - lowX) * (highY - lowY) * (highZ - lowZ);
	}else{
		return 0.0;
	}
}

void
kmb::BoxRegion::crossOnLine(const kmb::Point3D& origin, const kmb::Vector3D& dir, double &min_t, double &max_t) const
{
	kmb::BoundingBox1D bbox_x, bbox_y, bbox_z;
	if( dir.x() != 0.0 ){
		bbox_x.update( (minPoint.x() - origin.x()) / dir.x() );
		bbox_x.update( (maxPoint.x() - origin.x()) / dir.x() );
	}else if( minPoint.x() <= origin.x() && origin.x() <= maxPoint.x() ){
		bbox_x.update( DBL_MAX );
		bbox_x.update( -DBL_MAX );
	}
	if( dir.y() != 0.0 ){
		bbox_y.update( (minPoint.y() - origin.y()) / dir.y() );
		bbox_y.update( (maxPoint.y() - origin.y()) / dir.y() );
	}else if( minPoint.y() <= origin.y() && origin.y() <= maxPoint.y() ){
		bbox_y.update( DBL_MAX );
		bbox_y.update( -DBL_MAX );
	}
	if( dir.z() != 0.0 ){
		bbox_z.update( (minPoint.z() - origin.z()) / dir.z() );
		bbox_z.update( (maxPoint.z() - origin.z()) / dir.z() );
	}else if( minPoint.z() <= origin.z() && origin.z() <= maxPoint.z() ){
		bbox_z.update( DBL_MAX );
		bbox_z.update( -DBL_MAX );
	}
	// [t_minx,t_maxx] と [t_miny,t_maxy] の積を min_t max_t にする
	if( bbox_x.valid() ){
		if( bbox_y.valid() ){
			if( bbox_z.valid() ){
				min_t = kmb::Maximizer::getMax( bbox_x.getMin(), bbox_y.getMin(), bbox_z.getMin() );
				max_t = kmb::Minimizer::getMin( bbox_x.getMax(), bbox_y.getMax(), bbox_z.getMax() );
				return;
			}
		}
	}
	min_t = 0.0;
	max_t = 0.0;
}

bool
kmb::BoxRegion::intersect(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const
{
	// 点が含まれているときは無条件で true
	if( intersect( a ) == kmb::Region::INSIDE ||
		intersect( b ) == kmb::Region::INSIDE ||
		intersect( c ) == kmb::Region::INSIDE )
	{
		return true;
	}
	int countX(0), countY(0), countZ(0);
	double d = 0.0;
	double localThres = thres * this->range();
	kmb::Matrix3x3 mat;
	kmb::Vector3D v;
	kmb::Vector3D t;
	// ab,bc,ca と6面の判定
	// yz
	d = a.x() - b.x();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(1,1,rangeY());
		mat.set(2,2,rangeZ());
		// ab vs (x0,y0,z0)-yz
		// a + tx(b-a) = (x0,y0,z0) + ty(0,y1-y0,0) + tz(0,0,z1-z0)
		mat.set(0,0,a.x()-b.x());
		mat.set(1,0,a.y()-b.y());
		mat.set(2,0,a.z()-b.z());
		v.x( a.x()-minPoint.x() );
		v.y( a.y()-minPoint.y() );
		v.z( a.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// ab vs (x1,y0,z0)-yz
		// a + tx(b-a) = (x1,y0,z0) + ty(0,y1-y0,0) + tz(0,0,z1-z0)
		v.x( a.x()-maxPoint.x() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// ab が yz に平行な場合
		++countX;
	}
	d = b.x() - c.x();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(1,1,rangeY());
		mat.set(2,2,rangeZ());
		// bc vs (x0,y0,z0)-yz
		// b + tx(c-b) = (x0,y0,z0) + ty(0,y1-y0,0) + tz(0,0,z1-z0)
		mat.set(0,0,b.x()-c.x());
		mat.set(1,0,b.y()-c.y());
		mat.set(2,0,b.z()-c.z());
		v.x( b.x()-minPoint.x() );
		v.y( b.y()-minPoint.y() );
		v.z( b.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// bc vs (x1,y0,z0)-yz
		// b + tx(c-b) = (x1,y0,z0) + ty(0,y1-y0,0) + tz(0,0,z1-z0)
		v.x( b.x()-maxPoint.x() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// bc が yz に平行な場合
		++countX;
	}
	d = c.x() - a.x();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(1,1,rangeY());
		mat.set(2,2,rangeZ());
		// ca vs (x0,y0,z0)-yz
		// c + tx(a-c) = (x0,y0,z0) + ty(0,y1-y0,0) + tz(0,0,z1-z0)
		mat.set(0,0,c.x()-a.x());
		mat.set(1,0,c.y()-a.y());
		mat.set(2,0,c.z()-a.z());
		v.x( c.x()-minPoint.x() );
		v.y( c.y()-minPoint.y() );
		v.z( c.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// ca vs (x1,y0,z0)-yz
		// c + tx(a-c) = (x1,y0,z0) + ty(0,y1-y0,0) + tz(0,0,z1-z0)
		v.x( c.x()-maxPoint.x() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// ca が yz に平行な場合
		++countX;
	}
	if( countX == 3 ){
		if( -localThres + minX() < a.x() && a.x() < maxX() + localThres &&
			-localThres + minX() < b.x() && b.x() < maxX() + localThres &&
			-localThres + minX() < c.x() && c.x() < maxX() + localThres )
		{
			// yz 平面
			kmb::BoundingBox2D bbox2;
			bbox2.update( a.y(), a.z() );
			bbox2.update( b.y(), b.z() );
			bbox2.update( c.y(), c.z() );
			kmb::BoxRegion2D bboxYZ( minY(), minZ(), maxY(), maxZ() );
			if( bbox2.intersect( bboxYZ ) ){
				return true;
			}
		}
	}
	// zx
	d = a.y() - b.y();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(2,2,rangeZ());
		mat.set(0,0,rangeX());
		// ab vs (x0,y0,z0)-zx
		// a + ty(b-a) = (x0,y0,z0) + tx(x1-x0,0,0) + tz(0,0,z1-z0)
		mat.set(0,1,a.x()-b.x());
		mat.set(1,1,a.y()-b.y());
		mat.set(2,1,a.z()-b.z());
		v.x( a.x()-minPoint.x() );
		v.y( a.y()-minPoint.y() );
		v.z( a.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// ab vs (x0,y1,z0)-zx
		// a + ty(b-a) = (x0,y1,z0) + tx(x1-x0,0,0) + tz(0,0,z1-z0)
		v.y( a.y()-maxPoint.y() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// ab が zx に平行な場合
		++countY;
	}
	d = b.y() - c.y();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(2,2,rangeZ());
		mat.set(0,0,rangeX());
		// bc vs (x0,y0,z0)-zx
		// b + ty(c-b) = (x0,y0,z0) + tx(x1-x0,0,0) + tz(0,0,z1-z0)
		mat.set(0,1,b.x()-c.x());
		mat.set(1,1,b.y()-c.y());
		mat.set(2,1,b.z()-c.z());
		v.x( b.x()-minPoint.x() );
		v.y( b.y()-minPoint.y() );
		v.z( b.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// bc vs (x0,y1,z0)-zx
		// b + ty(c-b) = (x0,y1,z0) + tx(x1-x0,0,0) + tz(0,0,z1-z0)
		v.y( b.y()-maxPoint.y() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// bc が zx に平行な場合
		++countY;
	}
	d = c.y() - a.y();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(2,2,rangeZ());
		mat.set(0,0,rangeX());
		// ca vs (x0,y0,z0)-zx
		// c + ty(a-c) = (x0,y0,z0) + tx(x1-x0,0,0) + tz(0,0,z1-z0)
		mat.set(0,1,c.x()-a.x());
		mat.set(1,1,c.y()-a.y());
		mat.set(2,1,c.z()-a.z());
		v.x( c.x()-minPoint.x() );
		v.y( c.y()-minPoint.y() );
		v.z( c.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// ca vs (x0,y1,z0)-zx
		// c + ty(a-c) = (x0,y1,z0) + tx(x1-x0,0,0) + tz(0,0,z1-z0)
		v.y( c.y()-maxPoint.y() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// ca が zx に平行な場合
		++countY;
	}
	if( countY == 3 ){
		if( -localThres + minY() < a.y() && a.y() < maxY() + localThres &&
			-localThres + minY() < b.y() && b.y() < maxY() + localThres &&
			-localThres + minY() < c.y() && c.y() < maxY() + localThres )
		{
			// zx 平面
			kmb::BoundingBox2D bbox2;
			bbox2.update( a.z(), a.x() );
			bbox2.update( b.z(), b.x() );
			bbox2.update( c.z(), c.x() );
			kmb::BoxRegion2D bboxZX( minZ(), minX(), maxZ(), maxX() );
			if( bbox2.intersect( bboxZX ) ){
				return true;
			}
		}
	}
	// xy
	d = a.z() - b.z();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(0,0,rangeX());
		mat.set(1,1,rangeY());
		// ab vs (x0,y0,z0)-xy
		// a + tz(b-a) = (x0,y0,z0) + tx(x1-x0,0,0) + ty(0,y1-y0,0)
		mat.set(0,2,a.x()-b.x());
		mat.set(1,2,a.y()-b.y());
		mat.set(2,2,a.z()-b.z());
		v.x( a.x()-minPoint.x() );
		v.y( a.y()-minPoint.y() );
		v.z( a.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// ab vs (x0,y0,z1)-xy
		// a + tz(b-a) = (x0,y0,z1) + tx(x1-x0,0,0) + ty(0,y1-y0,0)
		v.z( a.z()-maxPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// ab が xy に平行な場合
		++countZ;
	}
	d = b.z() - c.z();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(0,0,rangeX());
		mat.set(1,1,rangeY());
		// bc vs (x0,y0,z0)-xy
		// b + tz(c-b) = (x0,y0,z0) + tx(x1-x0,0,0) + ty(0,y1-y0,0)
		mat.set(0,2,b.x()-c.x());
		mat.set(1,2,b.y()-c.y());
		mat.set(2,2,b.z()-c.z());
		v.x( b.x()-minPoint.x() );
		v.y( b.y()-minPoint.y() );
		v.z( b.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// bc vs (x0,y0,z1)-xy
		// b + tz(c-b) = (x0,y0,z1) + tx(x1-x0,0,0) + ty(0,y1-y0,0)
		v.z( b.z()-maxPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// bc が xy に平行な場合
		++countZ;
	}
	d = c.z() - a.z();
	if( fabs(d) > localThres ){
		mat.zero();
		mat.set(0,0,rangeX());
		mat.set(1,1,rangeY());
		// ca vs (x0,y0,z0)-xy
		// c + tz(a-c) = (x0,y0,z0) + tx(x1-x0,0,0) + ty(0,y1-y0,0)
		mat.set(0,2,c.x()-a.x());
		mat.set(1,2,c.y()-a.y());
		mat.set(2,2,c.z()-a.z());
		v.x( c.x()-minPoint.x() );
		v.y( c.y()-minPoint.y() );
		v.z( c.z()-minPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
		// ca vs (x0,y0,z1)-xy
		// c + tz(a-c) = (x0,y0,z1) + tx(x1-x0,0,0) + ty(0,y1-y0,0)
		v.z( c.z()-maxPoint.z() );
		if( mat.solveSafely( v, t ) &&
			-localThres <= t.x() && t.x() <= 1.0 + localThres &&
			-localThres <= t.y() && t.y() <= 1.0 + localThres &&
			-localThres <= t.z() && t.z() <= 1.0 + localThres )
		{
			return true;
		}
	}else{
		// ca が xy に平行な場合
		++countZ;
	}
	if( countZ == 3 ){
		if( -localThres + minZ() < a.z() && a.z() < maxZ() + localThres &&
			-localThres + minZ() < b.z() && b.z() < maxZ() + localThres &&
			-localThres + minZ() < c.z() && c.z() < maxZ() + localThres )
		{
			// xy 平面
			kmb::BoundingBox2D bbox2;
			bbox2.update( a.x(), a.y() );
			bbox2.update( b.x(), b.y() );
			bbox2.update( c.x(), c.y() );
			kmb::BoxRegion2D bboxXY( minX(), minY(), maxX(), maxY() );
			if( bbox2.intersect( bboxXY ) ){
				return true;
			}
		}
	}
	// abc と辺の判定
	// PA = t0 BA + t1 CA + t2 PQ
	mat.zero();
	mat.set(0,0,a.x()-b.x());
	mat.set(1,0,a.y()-b.y());
	mat.set(2,0,a.z()-b.z());
	mat.set(0,1,a.x()-c.x());
	mat.set(1,1,a.y()-c.y());
	mat.set(2,1,a.z()-c.z());
	// x 軸方向
	mat.set(0,2,rangeX());
//	mat.set(1,2,0.0);
//	mat.set(2,2,0.0);
	v.x( a.x()-minPoint.x() );
	v.y( a.y()-minPoint.y() );
	v.z( a.z()-minPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
//	v.x( a.x()-minPoint.x() );
	v.y( a.y()-maxPoint.y() );
//	v.z( a.z()-minPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
//	v.x( a.x()-minPoint.x() );
//	v.y( a.y()-maxPoint.y() );
	v.z( a.z()-maxPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
//	v.x( a.x()-minPoint.x() );
	v.y( a.y()-minPoint.y() );
//	v.z( a.z()-maxPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
	// y 軸方向
	mat.set(0,2,0.0);
	mat.set(1,2,rangeY());
//	mat.set(2,2,0.0);
	v.zero();
	v.x( a.x()-minPoint.x() );
	v.y( a.y()-minPoint.y() );
	v.z( a.z()-minPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
	v.x( a.x()-maxPoint.x() );
//	v.y( a.y()-minPoint.y() );
//	v.z( a.z()-minPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
//	v.x( a.x()-maxPoint.x() );
//	v.y( a.y()-minPoint.y() );
	v.z( a.z()-maxPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
	v.x( a.x()-minPoint.x() );
//	v.y( a.y()-minPoint.y() );
//	v.z( a.z()-maxPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
	// z 軸方向
//	mat.set(0,2,0.0);
	mat.set(1,2,0.0);
	mat.set(2,2,rangeZ());
	v.zero();
	v.x( a.x()-minPoint.x() );
	v.y( a.y()-minPoint.y() );
	v.z( a.z()-minPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
	v.x( a.x()-maxPoint.x() );
//	v.y( a.y()-minPoint.y() );
//	v.z( a.z()-minPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
//	v.x( a.x()-maxPoint.x() );
	v.y( a.y()-maxPoint.y() );
//	v.z( a.z()-minPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
	v.x( a.x()-minPoint.x() );
//	v.y( a.y()-maxPoint.y() );
//	v.z( a.z()-minPoint.z() );
	if( mat.solveSafely( v, t ) &&
		-localThres <= t.x() && -localThres <= t.y() && t.x() + t.y() <= 1.0 + localThres &&
		-localThres <= t.z() && t.z() <= 1.0 + localThres )
	{
		return true;
	}
	if( countX == 3 || countY == 3 || countZ == 3){
//		REVOCAP_Debug_X("%d %d %d\n", countX, countY, countZ);
	}
	return false;
}

bool
kmb::BoxRegion::intersect3(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const
{
	return intersect_minx(a,b,c);
}

// 三角形が六面体に含まれる部分の面積を求める
//
// 三角形を平面で分割して、正の領域と負の領域に分ける
// x, y, z それぞれの軸に垂直な平面で min max で合計6回呼び出す
// 分けたものが四角形だったら、三角形に分けて呼び出す
double
kmb::BoxRegion::intersectArea(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const
{
	return intersectArea_minx(a,b,c);
}

double
kmb::BoxRegion::intersectArea_minx(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	// p3 p4 は切り口の座標
	kmb::Point3D p3, p4;
	switch( kmb::PlaneYZ::getIntersectionTriangle( minX(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_NEGATIVE: return 0.0;
	case kmb::Plane::kALL_POSITIVE: return kmb::BoxRegion::intersectArea_maxx(p0,p1,p2);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersectArea_maxx(p0,p3,p4);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersectArea_maxx(p1,p3,p4);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersectArea_maxx(p2,p3,p4);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersectArea_maxx(p1,p2,p4) + kmb::BoxRegion::intersectArea_maxx(p1,p4,p3);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersectArea_maxx(p2,p0,p4) + kmb::BoxRegion::intersectArea_maxx(p2,p4,p3);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersectArea_maxx(p0,p1,p4) + kmb::BoxRegion::intersectArea_maxx(p0,p4,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersectArea_maxx(p0,p1,p3);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersectArea_maxx(p1,p2,p3);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersectArea_maxx(p2,p0,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersectArea_maxx(p0,p3,p2);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersectArea_maxx(p1,p3,p0);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersectArea_maxx(p2,p3,p1);
	default: return 0.0;
	}
}

double
kmb::BoxRegion::intersectArea_maxx(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneYZ::getIntersectionTriangle( maxX(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_POSITIVE: return 0.0;
	case kmb::Plane::kALL_NEGATIVE: return kmb::BoxRegion::intersectArea_miny(p0,p1,p2);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersectArea_miny(p0,p3,p4);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersectArea_miny(p1,p3,p4);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersectArea_miny(p2,p3,p4);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersectArea_miny(p1,p2,p4) + kmb::BoxRegion::intersectArea_miny(p1,p4,p3);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersectArea_miny(p2,p0,p4) + kmb::BoxRegion::intersectArea_miny(p2,p4,p3);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersectArea_miny(p0,p1,p4) + kmb::BoxRegion::intersectArea_miny(p0,p4,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersectArea_miny(p0,p1,p3);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersectArea_miny(p1,p2,p3);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersectArea_miny(p2,p0,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersectArea_miny(p0,p3,p2);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersectArea_miny(p1,p3,p0);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersectArea_miny(p2,p3,p1);
	default: return 0.0;
	}
}

double
kmb::BoxRegion::intersectArea_miny(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneZX::getIntersectionTriangle( minY(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_NEGATIVE: return 0.0;
	case kmb::Plane::kALL_POSITIVE: return kmb::BoxRegion::intersectArea_maxy(p0,p1,p2);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersectArea_maxy(p0,p3,p4);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersectArea_maxy(p1,p3,p4);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersectArea_maxy(p2,p3,p4);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersectArea_maxy(p1,p2,p4) + kmb::BoxRegion::intersectArea_maxy(p1,p4,p3);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersectArea_maxy(p2,p0,p4) + kmb::BoxRegion::intersectArea_maxy(p2,p4,p3);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersectArea_maxy(p0,p1,p4) + kmb::BoxRegion::intersectArea_maxy(p0,p4,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersectArea_maxy(p0,p1,p3);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersectArea_maxy(p1,p2,p3);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersectArea_maxy(p2,p0,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersectArea_maxy(p0,p3,p2);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersectArea_maxy(p1,p3,p0);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersectArea_maxy(p2,p3,p1);
	default: return 0.0;
	}
}

double
kmb::BoxRegion::intersectArea_maxy(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneZX::getIntersectionTriangle( maxY(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_POSITIVE: return 0.0;
	case kmb::Plane::kALL_NEGATIVE: return kmb::BoxRegion::intersectArea_minz(p0,p1,p2);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersectArea_minz(p0,p3,p4);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersectArea_minz(p1,p3,p4);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersectArea_minz(p2,p3,p4);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersectArea_minz(p1,p2,p4) + kmb::BoxRegion::intersectArea_minz(p1,p4,p3);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersectArea_minz(p2,p0,p4) + kmb::BoxRegion::intersectArea_minz(p2,p4,p3);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersectArea_minz(p0,p1,p4) + kmb::BoxRegion::intersectArea_minz(p0,p4,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersectArea_minz(p0,p1,p3);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersectArea_minz(p1,p2,p3);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersectArea_minz(p2,p0,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersectArea_minz(p0,p3,p2);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersectArea_minz(p1,p3,p0);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersectArea_minz(p2,p3,p1);
	default: return 0.0;
	}
}

double
kmb::BoxRegion::intersectArea_minz(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneXY::getIntersectionTriangle( minZ(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_NEGATIVE: return 0.0;
	case kmb::Plane::kALL_POSITIVE: return kmb::BoxRegion::intersectArea_maxz(p0,p1,p2);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersectArea_maxz(p0,p3,p4);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersectArea_maxz(p1,p3,p4);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersectArea_maxz(p2,p3,p4);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersectArea_maxz(p1,p2,p4) + kmb::BoxRegion::intersectArea_maxz(p1,p4,p3);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersectArea_maxz(p2,p0,p4) + kmb::BoxRegion::intersectArea_maxz(p2,p4,p3);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersectArea_maxz(p0,p1,p4) + kmb::BoxRegion::intersectArea_maxz(p0,p4,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersectArea_maxz(p0,p1,p3);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersectArea_maxz(p1,p2,p3);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersectArea_maxz(p2,p0,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersectArea_maxz(p0,p3,p2);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersectArea_maxz(p1,p3,p0);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersectArea_maxz(p2,p3,p1);
	default: return 0.0;
	}
}

double
kmb::BoxRegion::intersectArea_maxz(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneXY::getIntersectionTriangle( maxZ(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_POSITIVE: return 0.0;
	case kmb::Plane::kALL_NEGATIVE: return kmb::Point3D::area(p0,p1,p2);
	case kmb::Plane::k1243_034: return kmb::Point3D::area(p0,p3,p4);
	case kmb::Plane::k2043_134: return kmb::Point3D::area(p1,p3,p4);
	case kmb::Plane::k0143_234: return kmb::Point3D::area(p2,p3,p4);
	case kmb::Plane::k034_1243: return kmb::Point3D::area(p1,p2,p4) + kmb::Point3D::area(p1,p4,p3);
	case kmb::Plane::k134_2043: return kmb::Point3D::area(p2,p0,p4) + kmb::Point3D::area(p2,p4,p3);
	case kmb::Plane::k234_0143: return kmb::Point3D::area(p0,p1,p4) + kmb::Point3D::area(p0,p4,p3);
	case kmb::Plane::k0_032_013: return kmb::Point3D::area(p0,p1,p3);
	case kmb::Plane::k1_130_123: return kmb::Point3D::area(p1,p2,p3);
	case kmb::Plane::k2_231_203: return kmb::Point3D::area(p2,p0,p3);
	case kmb::Plane::k0_013_032: return kmb::Point3D::area(p0,p3,p2);
	case kmb::Plane::k1_123_130: return kmb::Point3D::area(p1,p3,p0);
	case kmb::Plane::k2_203_231: return kmb::Point3D::area(p2,p3,p1);
	default: return 0.0;
	}
}

bool
kmb::BoxRegion::intersect_minx(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneYZ::getIntersectionTriangle( minX(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_NEGATIVE: return false;
	case kmb::Plane::kALL_POSITIVE: return kmb::BoxRegion::intersect_maxx(p0,p1,p2);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersect_maxx(p0,p3,p4);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersect_maxx(p1,p3,p4);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersect_maxx(p2,p3,p4);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersect_maxx(p1,p2,p4) || kmb::BoxRegion::intersect_maxx(p1,p4,p3);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersect_maxx(p2,p0,p4) || kmb::BoxRegion::intersect_maxx(p2,p4,p3);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersect_maxx(p0,p1,p4) || kmb::BoxRegion::intersect_maxx(p0,p4,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersect_maxx(p0,p1,p3);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersect_maxx(p1,p2,p3);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersect_maxx(p2,p0,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersect_maxx(p0,p3,p2);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersect_maxx(p1,p3,p0);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersect_maxx(p2,p3,p1);
	default: return false;
	}
}

bool
kmb::BoxRegion::intersect_maxx(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneYZ::getIntersectionTriangle( maxX(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_POSITIVE: return false;
	case kmb::Plane::kALL_NEGATIVE: return kmb::BoxRegion::intersect_miny(p0,p1,p2);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersect_miny(p0,p3,p4);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersect_miny(p1,p3,p4);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersect_miny(p2,p3,p4);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersect_miny(p1,p2,p4) || kmb::BoxRegion::intersectArea_miny(p1,p4,p3);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersect_miny(p2,p0,p4) || kmb::BoxRegion::intersectArea_miny(p2,p4,p3);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersect_miny(p0,p1,p4) || kmb::BoxRegion::intersectArea_miny(p0,p4,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersect_miny(p0,p1,p3);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersect_miny(p1,p2,p3);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersect_miny(p2,p0,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersect_miny(p0,p3,p2);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersect_miny(p1,p3,p0);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersect_miny(p2,p3,p1);
	default: return false;
	}
}

bool
kmb::BoxRegion::intersect_miny(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneZX::getIntersectionTriangle( minY(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_NEGATIVE: return false;
	case kmb::Plane::kALL_POSITIVE: return kmb::BoxRegion::intersect_maxy(p0,p1,p2);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersect_maxy(p0,p3,p4);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersect_maxy(p1,p3,p4);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersect_maxy(p2,p3,p4);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersect_maxy(p1,p2,p4) || kmb::BoxRegion::intersectArea_maxy(p1,p4,p3);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersect_maxy(p2,p0,p4) || kmb::BoxRegion::intersectArea_maxy(p2,p4,p3);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersect_maxy(p0,p1,p4) || kmb::BoxRegion::intersectArea_maxy(p0,p4,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersect_maxy(p0,p1,p3);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersect_maxy(p1,p2,p3);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersect_maxy(p2,p0,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersect_maxy(p0,p3,p2);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersect_maxy(p1,p3,p0);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersect_maxy(p2,p3,p1);
	default: return false;
	}
}

bool
kmb::BoxRegion::intersect_maxy(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneZX::getIntersectionTriangle( maxY(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_POSITIVE: return false;
	case kmb::Plane::kALL_NEGATIVE: return kmb::BoxRegion::intersect_minz(p0,p1,p2);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersect_minz(p0,p3,p4);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersect_minz(p1,p3,p4);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersect_minz(p2,p3,p4);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersect_minz(p1,p2,p4) || kmb::BoxRegion::intersect_minz(p1,p4,p3);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersect_minz(p2,p0,p4) || kmb::BoxRegion::intersect_minz(p2,p4,p3);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersect_minz(p0,p1,p4) || kmb::BoxRegion::intersect_minz(p0,p4,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersect_minz(p0,p1,p3);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersect_minz(p1,p2,p3);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersect_minz(p2,p0,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersect_minz(p0,p3,p2);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersect_minz(p1,p3,p0);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersect_minz(p2,p3,p1);
	default: return false;
	}
}

bool
kmb::BoxRegion::intersect_minz(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneXY::getIntersectionTriangle( minZ(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_NEGATIVE: return false;
	case kmb::Plane::kALL_POSITIVE: return kmb::BoxRegion::intersect_maxz(p0,p1,p2);
	case kmb::Plane::k034_1243: return kmb::BoxRegion::intersect_maxz(p0,p3,p4);
	case kmb::Plane::k134_2043: return kmb::BoxRegion::intersect_maxz(p1,p3,p4);
	case kmb::Plane::k234_0143: return kmb::BoxRegion::intersect_maxz(p2,p3,p4);
	case kmb::Plane::k1243_034: return kmb::BoxRegion::intersect_maxz(p1,p2,p4) || kmb::BoxRegion::intersect_maxz(p1,p4,p3);
	case kmb::Plane::k2043_134: return kmb::BoxRegion::intersect_maxz(p2,p0,p4) || kmb::BoxRegion::intersect_maxz(p2,p4,p3);
	case kmb::Plane::k0143_234: return kmb::BoxRegion::intersect_maxz(p0,p1,p4) || kmb::BoxRegion::intersect_maxz(p0,p4,p3);
	case kmb::Plane::k0_013_032: return kmb::BoxRegion::intersect_maxz(p0,p1,p3);
	case kmb::Plane::k1_123_130: return kmb::BoxRegion::intersect_maxz(p1,p2,p3);
	case kmb::Plane::k2_203_231: return kmb::BoxRegion::intersect_maxz(p2,p0,p3);
	case kmb::Plane::k0_032_013: return kmb::BoxRegion::intersect_maxz(p0,p3,p2);
	case kmb::Plane::k1_130_123: return kmb::BoxRegion::intersect_maxz(p1,p3,p0);
	case kmb::Plane::k2_231_203: return kmb::BoxRegion::intersect_maxz(p2,p3,p1);
	default: return false;
	}
}

bool
kmb::BoxRegion::intersect_maxz(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const
{
	kmb::Point3D p3, p4;
	switch( kmb::PlaneXY::getIntersectionTriangle( maxZ(), p0, p1, p2, p3, p4 ) )
	{
	case kmb::Plane::kALL_POSITIVE:
		return false;
	case kmb::Plane::kALL_NEGATIVE:
	case kmb::Plane::k1243_034:
	case kmb::Plane::k2043_134:
	case kmb::Plane::k0143_234:
	case kmb::Plane::k034_1243:
	case kmb::Plane::k134_2043:
	case kmb::Plane::k234_0143:
	case kmb::Plane::k0_032_013:
	case kmb::Plane::k1_130_123:
	case kmb::Plane::k2_231_203:
	case kmb::Plane::k0_013_032:
	case kmb::Plane::k1_123_130:
	case kmb::Plane::k2_203_231:
		return true;
	default:
		return false;
	}
}

double
kmb::BoxRegion::distanceSq(const kmb::BoxRegion& box) const
{
	return this->maxPoint.distanceSq( box.maxPoint ) + this->minPoint.distanceSq( box.minPoint );
}

void
kmb::BoxRegion::expand(double x,double y,double z)
{
	kmb::Vector3D diff(x,y,z);
	this->maxPoint += diff;
	this->minPoint -= diff;
}

void
kmb::BoxRegion::setRange(double rangeX,double rangeY,double rangeZ)
{
	double cx = this->centerX();
	double cy = this->centerY();
	double cz = this->centerZ();
	if( rangeX < 0.0 ){ rangeX *= -1.0; }
	if( rangeY < 0.0 ){ rangeY *= -1.0; }
	if( rangeZ < 0.0 ){ rangeZ *= -1.0; }
	this->maxPoint.setCoordinate( cx + 0.5*rangeX, cy + 0.5*rangeY, cz + 0.5*rangeZ );
	this->minPoint.setCoordinate( cx - 0.5*rangeX, cy - 0.5*rangeY, cz - 0.5*rangeZ );
}

// 縦横の長さを ratio 倍する
// ただし ratio > 0
void
kmb::BoxRegion::expand(double ratio)
{
	if( ratio > 0.0 && ratio != 1.0)
	{
		double maxx = maxPoint.x();
		double minx = minPoint.x();
		maxPoint.x( 0.5*(1+ratio)*maxx + 0.5*(1-ratio)*minx );
		minPoint.x( 0.5*(1-ratio)*maxx + 0.5*(1+ratio)*minx );
		double maxy = maxPoint.y();
		double miny = minPoint.y();
		maxPoint.y( 0.5*(1+ratio)*maxy + 0.5*(1-ratio)*miny );
		minPoint.y( 0.5*(1-ratio)*maxy + 0.5*(1+ratio)*miny );
		double maxz = maxPoint.z();
		double minz = minPoint.z();
		maxPoint.z( 0.5*(1+ratio)*maxz + 0.5*(1-ratio)*minz );
		minPoint.z( 0.5*(1-ratio)*maxz + 0.5*(1+ratio)*minz );
	}
}

void
kmb::BoxRegion::translate(double x,double y,double z)
{
	minPoint.addCoordinate(x,y,z);
	maxPoint.addCoordinate(x,y,z);
}

void
kmb::BoxRegion::expandDiameter(double ratio)
{
	if( ratio > 1.0 ){
		double d = rangeX() + rangeY() + rangeZ();
		double w = 1.0/6.0 * ( sqrt( d*d + 3.0 * this->diameterSq() * (ratio*ratio-1.0) ) -d );
		maxPoint.addCoordinate( w, w, w );
		minPoint.addCoordinate( -w, -w, -w );
	}
}
