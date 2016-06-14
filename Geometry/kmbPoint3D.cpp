﻿/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : Point3D                                                 #
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
#include <cmath>
#include <cfloat>
#include <cstdio>
#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbGeometry2D.h"
#include "Common/kmbCalculator.h"
#include "Geometry/kmbGeometry4D.h"

#include <iostream>

kmb::Point3D kmb::Point3D::infinity(DBL_MAX,DBL_MAX,DBL_MAX);

// copy constructor
// kmb::Point3D p = other;
// の時にも呼ばれる
kmb::Point3D::Point3D(const Tuple3D &other) : Tuple3D(other){}

kmb::Point3D::~Point3D(void){}

kmb::Point3D
kmb::Point3D::operator+(const Vector3D& other) const
{
	return Point3D(v[0]+other.x(),v[1]+other.y(),v[2]+other.z());
}

kmb::Point3D
kmb::Point3D::operator-(const Vector3D& other) const
{
	return Point3D(v[0]-other.x(),v[1]-other.y(),v[2]-other.z());
}

kmb::Point3D&
kmb::Point3D::operator+=(const Vector3D& other)
{
	this->v[0] += other.x();
	this->v[1] += other.y();
	this->v[2] += other.z();
	return *this;
}

kmb::Point3D&
kmb::Point3D::operator-=(const Vector3D& other)
{
	this->v[0] -= other.x();
	this->v[1] -= other.y();
	this->v[2] -= other.z();
	return *this;
}

bool
kmb::Point3D::operator==(const Point3D& other) const
{
	return (this->x() == other.x()) && (this->y() == other.y()) && (this->z() == other.z());
}

kmb::Point3D&
kmb::Point3D::operator =(const Point3D& other)
{
	this->v[0] = other.x();
	this->v[1] = other.y();
	this->v[2] = other.z();
	return *this;
}

double
kmb::Point3D::distance(const Point3D& other) const
{
	return this->distance(other.x(),other.y(),other.z());
}

double
kmb::Point3D::distanceSq(const Point3D& other) const
{
	return this->distanceSq(other.x(),other.y(),other.z());
}

double
kmb::Point3D::distance(double x,double y,double z) const
{
	return sqrt( distanceSq(x,y,z) );
}

double
kmb::Point3D::distanceSq(double x,double y,double z) const
{
	return 
		(this->x()-x)*(this->x()-x) + 
		(this->y()-y)*(this->y()-y) +
		(this->z()-z)*(this->z()-z); 
}

double
kmb::Point3D::distanceSqToSegment(const Point3D& a,const Point3D& b) const
{
	double t;
	return distanceSqToSegment(a,b,t);
}

double
kmb::Point3D::distanceSqToSegment(const Point3D& a,const Point3D& b,double& t) const
{
	kmb::Vector3D ab(b,a);
	kmb::Vector3D xa(a,*this);   // x = this
	double len = ab.lengthSq();
	if( len != 0.0 ){
		t = - (ab * xa) / len;
	}else{
		// a と b が一致している
		t = 0.0;
	}
	if( t < 0 ){
		t = 0.0;
		return this->distanceSq(a);
	}else if( t > 1 ){
		t = 1.0;
		return this->distanceSq(b);
	}else{
		return ( xa + ab.scalar(t) ).lengthSq();
	}
}

double
kmb::Point3D::distanceToSegment(const Point3D& a,const Point3D& b) const
{
	return sqrt( distanceSqToSegment(a,b) );
}

// u と v が平行の時は決まらないので false
bool
kmb::Point3D::getFootOfPerpendicular( const Point3D &origin, const Vector3D &u, const Vector3D &v, double &a, double &b) const
{
	kmb::Vector3D w( *this, origin );
	double q = u * v;
	kmb::Matrix2x2 mat(u.lengthSq(),q,q,v.lengthSq());
	kmb::Vector2D* t = mat.solve( kmb::Vector2D(u*w,v*w) );
	if( t ){
		a = t->getCoordinate(0);
		b = t->getCoordinate(1);
		delete t;
		return true;
	}
	return false;
}

double
kmb::Point3D::distanceSqToTriangle(const Point3D& a,const Point3D& b,const Point3D& c,double *t) const
{
	// this と a + t1(b-a) + t2(c-a) との距離の最小化
	// u0 + t1*u1 + t2*u2 の長さの最小化
	kmb::Vector3D u0(a,*this);
	kmb::Vector3D u1(b,a);
	kmb::Vector3D u2(c,a);
	kmb::Matrix2x2 mat(
		u1*u1, u1*u2,
		u2*u1, u2*u2);
	kmb::Vector2D v(-(u1*u0),-(u2*u0));
	kmb::Vector2D* tmp = mat.solve(v);
	if( tmp && 
		0.0 < tmp->x() && 0.0 < tmp->y() &&
		tmp->x() + tmp->y() < 1.0 )
	{
		if( t != NULL ){
			t[0] = (*tmp)[0];
			t[1] = (*tmp)[1];
		}
		double distSq = (u0 + u1.scalar((*tmp)[0]) + u2.scalar((*tmp)[1])).lengthSq();
		delete tmp;
		return distSq;
	}else{
		// 境界で最小を取る
		// 辺と点
		kmb::Minimizer min;
		double s=0.0;
		if( min.update( this->distanceSqToSegment( a, b, s ) ) ){
			if( t != NULL ){
				t[0] = s;
				t[1] = 0.0;
			}
		}
		if( min.update( this->distanceSqToSegment( b, c, s ) ) ){
			if( t != NULL ){
				t[0] = 1.0-s;
				t[1] = s;
			}
		}
		if( min.update( this->distanceSqToSegment( c, a, s ) ) ){
			if( t != NULL ){
				t[0] = 0.0;
				t[1] = 1.0-s;
			}
		}
		if( tmp ){
			delete tmp;
		}
		return min.getMin();
	}
}

double
kmb::Point3D::distanceToTriangle(const Point3D& a,const Point3D& b,const Point3D& c) const
{
	return sqrt( distanceSqToTriangle(a,b,c) );
}

// 内分点
kmb::Point3D
kmb::Point3D::dividingPoint(const Point3D& other,double m,double n) const
{
	return Point3D(
		(n*this->x() + m*other.x()) / (m+n), 
		(n*this->y() + m*other.y()) / (m+n), 
		(n*this->z() + m*other.z()) / (m+n)); 
}

kmb::Point3D
kmb::Point3D::proportionalPoint(const Point3D& other,double t) const
{
	return Point3D(
		(1.0-t)*this->x() + t*other.x(), 
		(1.0-t)*this->y() + t*other.y(), 
		(1.0-t)*this->z() + t*other.z()); 
}

#ifndef REVOCAP_SUPPORT_RUBY
kmb::Point3D kmb::Point3D::dividingPoint(const Point3D& a,const Point3D& b,double m,double n)
{
	return a.dividingPoint(b,m,n);
}

kmb::Point3D kmb::Point3D::proportionalPoint(const Point3D& a,const Point3D& b,double t)
{
	return a.proportionalPoint(b,t);
}

double
kmb::Point3D::distance(const Point3D& a,const Point3D& b)
{
	return a.distance(b);
}

double
kmb::Point3D::distanceSq(const Point3D& a,const Point3D& b)
{
	return a.distanceSq(b);
}
#endif

// 体積計算
double
kmb::Point3D::volume(const kmb::Point3D& a,const kmb::Point3D& b,const kmb::Point3D &c,const kmb::Point3D &d)
{
	kmb::Vector3D ad(d,a);
	kmb::Vector3D bd(d,b);
	kmb::Vector3D cd(d,c);

	return kmb::Vector3D::triple(ad,bd,cd) / 6.0;
}

// 表裏判定（遅いかも）
// abcx の四面体の体積が正になる方向がプラス
bool 
kmb::Point3D::positive(const Point3D& a,const Point3D& b,const Point3D &c,const Point3D &x)
{
	kmb::Vector3D ax(x,a);
	kmb::Vector3D bx(x,b);
	kmb::Vector3D cx(x,c);

	return (kmb::Vector3D::triple(ax,bx,cx) > 0);
}

// 面積
double
kmb::Point3D::area(const kmb::Point3D& a,const kmb::Point3D& b,const kmb::Point3D &c)
{
	kmb::Vector3D ab(b,a);
	kmb::Vector3D ac(c,a);
	kmb::Vector3D cross = ab%ac;
	return cross.length() * 0.5;
}

kmb::Vector3D
kmb::Point3D::areaVector(const kmb::Point3D& a,const kmb::Point3D& b,const kmb::Point3D &c)
{
	kmb::Vector3D ab(b,a); // b-a
	kmb::Vector3D ac(c,a); // c-a
	return 0.5*ab%ac;
}

kmb::Point3D kmb::Point3D::getCenter(const Point3D& a,const Point3D& b)
{
	return kmb::Point3D( (a.x() + b.x())*0.5, (a.y() + b.y())*0.5, (a.z() + b.z())*0.5 );
}

kmb::Point3D kmb::Point3D::getCenter(const Point3D& a,const Point3D& b,const Point3D &c)
{
	double r = 1.0/3.0;
	return kmb::Point3D( (a.x() + b.x() + c.x())*r, (a.y() + b.y() + c.y())*r, (a.z() + b.z() + c.z())*r );
}

kmb::Point3D kmb::Point3D::getCenter(const Point3D& a,const Point3D& b,const Point3D &c,const Point3D &d)
{
	double r = 1.0/4.0;
	return kmb::Point3D( (a.x() + b.x() + c.x() + d.x())*r, (a.y() + b.y() + c.y() + d.y())*r, (a.z() + b.z() + c.z() + d.z())*r );
}

kmb::Point3D kmb::Point3D::getCircumCenter(const Point3D& a,const Point3D& b,const Point3D &c)
{
	double ec = kmb::Point3D::distanceSq(a,b);
	double ea = kmb::Point3D::distanceSq(b,c);
	double eb = kmb::Point3D::distanceSq(c,a);
	double aa = ea*(eb+ec-ea);
	double bb = eb*(ec+ea-eb);
	double cc = ec*(ea+eb-ec);
	double r = 1.0 / (aa+bb+cc);
	return kmb::Point3D(
		r * ( aa*a.x()+bb*b.x()+cc*c.x() ),
		r * ( aa*a.y()+bb*b.y()+cc*c.y() ),
		r * ( aa*a.z()+bb*b.z()+cc*c.z() ));
}

kmb::Point3D kmb::Point3D::getCircumCenter(const Point3D &p0,const Point3D &p1,const Point3D &p2,const Point3D &p3)
{
	double sq0 = p0.x()*p0.x() + p0.y()*p0.y() + p0.z()*p0.z();
	double sq1 = p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z();
	double sq2 = p2.x()*p2.x() + p2.y()*p2.y() + p2.z()*p2.z();
	double sq3 = p3.x()*p3.x() + p3.y()*p3.y() + p3.z()*p3.z();
	double rx = kmb::Matrix4x4::determinant(
			sq0, 1.0, p0.y(), p0.z(),
			sq1, 1.0, p1.y(), p1.z(),
			sq2, 1.0, p2.y(), p2.z(),
			sq3, 1.0, p3.y(), p3.z()
		);
	double ry = kmb::Matrix4x4::determinant(
			sq0, p0.x(), 1.0, p0.z(),
			sq1, p1.x(), 1.0, p1.z(),
			sq2, p2.x(), 1.0, p2.z(),
			sq3, p3.x(), 1.0, p3.z()
		);
	double rz = kmb::Matrix4x4::determinant(
			sq0, p0.x(), p0.y(), 1.0,
			sq1, p1.x(), p1.y(), 1.0,
			sq2, p2.x(), p2.y(), 1.0,
			sq3, p3.x(), p3.y(), 1.0
		);
	double r = -0.5 / kmb::Matrix4x4::determinant(
			1.0, p0.x(), p0.y(), p0.z(),
			1.0, p1.x(), p1.y(), p1.z(),
			1.0, p2.x(), p2.y(), p2.z(),
			1.0, p3.x(), p3.y(), p3.z()
		);
	return kmb::Point3D( r*rx, r*ry, r*rz );
}

double kmb::Point3D::angle(const Point3D &a,const Point3D &b,const Point3D &c)
{
	kmb::Vector3D v1(a,b);
	kmb::Vector3D v2(c,b);
	return kmb::Vector3D::angle(v1,v2);
}

double
kmb::Point3D::cos(const Point3D &a,const Point3D &b,const Point3D &c)
{
	kmb::Vector3D v1(a,b);
	kmb::Vector3D v2(c,b);
	return kmb::Vector3D::cos(v1,v2);
}

double
kmb::Point3D::sin(const Point3D &a,const Point3D &b,const Point3D &c)
{
	kmb::Vector3D v1(a,b);
	kmb::Vector3D v2(c,b);
	return kmb::Vector3D::sin(v1,v2);
}

void 
kmb::Point3D::calcMinorCoordinate
( const kmb::Point3D& a, const kmb::Point3D& b, const kmb::Point3D& c, const kmb::Point3D& d, const kmb::Point3D& x, double coordinate[4])
{
	kmb::Vector3D xa(a,x);
	kmb::Vector3D xb(b,x);
	kmb::Vector3D xc(c,x);
	kmb::Vector3D xd(d,x);
	// xbcd
	coordinate[0] = kmb::Vector3D::triple(xb,xc,xd) / 6.0;
	// -axcd
	coordinate[1] = -kmb::Vector3D::triple(xa,xc,xd) / 6.0;
	// abxd
	coordinate[2] = kmb::Vector3D::triple(xa,xb,xd) / 6.0;
	// -abcx
	coordinate[3] = -kmb::Vector3D::triple(xa,xb,xc) / 6.0;
}

kmb::Vector3D
kmb::Point3D::calcNormalVector( const kmb::Point3D& a, const kmb::Point3D& b, const kmb::Point3D& c )
{
	kmb::Vector3D v1(a,b);
	kmb::Vector3D v2(a,c);
	kmb::Vector3D v = v1 % v2;
	v.normalize();
	return v;
}

//
// 4角形の法線ベクトルは、対角線で三角形2個に分割した場合の面積の重み付きの法線ベクトルの和
// これは対角線の外積と一致する
//
kmb::Vector3D
kmb::Point3D::calcNormalVector( const kmb::Point3D& a, const kmb::Point3D& b, const kmb::Point3D& c, const kmb::Point3D& d )
{
	kmb::Vector3D v1(a,c);
	kmb::Vector3D v2(b,d);
	kmb::Vector3D v = v1 % v2;
	v.normalize();
	return v;
}

kmb::Vector3D
kmb::Point3D::calcNormalVector(
		const double x0, const double y0, const double z0,
		const double x1, const double y1, const double z1,
		const double x2, const double y2, const double z2)
{
	kmb::Vector3D u(x0-x1,y0-y1,z0-z1);
	kmb::Vector3D v(x0-x2,y0-y2,z0-z2);
	kmb::Vector3D normal = u%v;
	normal.normalize();
	return normal;
}

kmb::Vector3D
kmb::Point3D::calcNormalVector(
		const double x0, const double y0, const double z0,
		const double x1, const double y1, const double z1,
		const double x2, const double y2, const double z2,
		const double x3, const double y3, const double z3)
{
	kmb::Vector3D u(x0-x2,y0-y2,z0-z2);
	kmb::Vector3D v(x1-x3,y1-y3,z1-z3);
	kmb::Vector3D normal = u%v;
	normal.normalize();
	return normal;
}


