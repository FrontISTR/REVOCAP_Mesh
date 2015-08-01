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
#pragma once

#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbGeometry2D.h"

namespace kmb{

/**
 * ��ԓ��̕���
 * ax + by + cz + d = 0
 * a*a + b*b + c*c = 1 �ƂȂ�悤�ɐ��K�����Ă���
 */

class Plane
{
protected:
	double a;
	double b;
	double c;
	double d;
public:
	Plane(const double a,const double b,const double c,const double d);
	Plane(const kmb::Point3D &base,const kmb::Vector3D &normal);
	virtual ~Plane(void);
	static Plane* createFromPoints(const Point3D &p,const Point3D &q,const Point3D &r);
	static Plane* createFromBaseNormal(const Point3D &base,const Vector3D &normal);
	static Vector3D calcNormal(const Point3D &a,const Point3D &b,const Point3D &c);
	double distance(const Point3D &pt) const;
	double distance(const double x,const double y,const double z) const;
	double evaluate(const double x,const double y,const double z) const;
	double evaluate(const Point3D &pt) const;
	Vector3D getNormal(void) const;
	double getConstant(void) const;
	// �@���̕ύX
	bool setNormal(double a,double b,double c);
	void setOrigin(double x,double y,double z);
	// ��W����������Ȃ�����|�C���^�ŕԂ�
	Point3D* createIntersectPoint(const Point3D &p0,const Point3D &p1) const;
	// p0 �� p1 �����Ԓ����Ƃ̌�_
	// p0 �� t=0�Ap1 �� t=1 �Ƃ��ăp�����[�^��^�����Ƃ��� t ���v�Z����
	bool getIntersection(const Point3D &p0,const Point3D &p1, Point3D &pt, double &t) const;
	// p0 p1 p2 �̎O�p�`�Ƃ̌����
	// �߂�l
	// -2 �����Ȃ��O�p�`�͕��̑�
	// -1 �����Ȃ��O�p�`�͐��̑�
	// 0 p0 p3 p4 �����̑� p1 p2 p4 p3 �����̑�
	// 1 p1 p3 p4 �����̑� p2 p0 p4 p3 �����̑�
	// 2 p2 p3 p4 �����̑� p0 p1 p4 p3 �����̑�
	// 3 p1 p2 p4 p3 �����̑� p0 p3 p4 �����̑�
	// 4 p2 p0 p4 p3 �����̑� p1 p3 p4 �����̑�
	// 5 p0 p1 p4 p3 �����̑� p2 p3 p4 �����̑�
	// 6 p0 ��ʂ� p0 p1 p3 �����̑� p0 p3 p2 �����̑�
	// 7 p1 ��ʂ� p1 p2 p3 �����̑� p1 p3 p0 �����̑�
	// 8 p2 ��ʂ� p2 p0 p3 �����̑� p2 p3 p1 �����̑�
	// 9  p0 ��ʂ� p0 p3 p2 �����̑� p0 p1 p3 �����̑�
	// 10 p1 ��ʂ� p1 p3 p0 �����̑� p1 p2 p3 �����̑�
	// 11 p2 ��ʂ� p2 p3 p1 �����̑� p2 p0 p3 �����̑�
	int getIntersectionTriangle(const Point3D &p0,const Point3D &p1, const Point3D &p2, Point3D &p3, Point3D &p4) const;
	// get foot of perpendicular
	Point3D projectOnPlane(const Point3D& p) const;
};

class PlaneXY : public Plane
{
public:
	PlaneXY(const double z0) : Plane( 0.0, 0.0, 1.0, -z0){};
	virtual ~PlaneXY(void){};
	void setZ(double z0);
	static bool getIntersection(const double z0,const Point3D &p0,const Point3D &p1, Point3D &pt, double &t);
	static int getIntersectionTriangle(const double z0,const Point3D &p0,const Point3D &p1, const Point3D &p2, Point3D &p3, Point3D &p4);
};

class PlaneYZ : public Plane
{
public:
	PlaneYZ(const double x0) : Plane( 1.0, 0.0, 0.0, -x0){};
	virtual ~PlaneYZ(void){};
	void setX(double x0);
	static bool getIntersection(const double x0,const Point3D &p0,const Point3D &p1, Point3D &pt, double &t);
	static int getIntersectionTriangle(const double x0,const Point3D &p0,const Point3D &p1, const Point3D &p2, Point3D &p3, Point3D &p4);
};

class PlaneZX : public Plane
{
public:
	PlaneZX(const double y0) : Plane( 0.0, 1.0, 0.0, -y0){};
	virtual ~PlaneZX(void){};
	void setY(double y0);
	static bool getIntersection(const double y0,const Point3D &p0,const Point3D &p1, Point3D &pt, double &t);
	static int getIntersectionTriangle(
		const double y0,const Point3D &p0,const Point3D &p1, const Point3D &p2, Point3D &p3, Point3D &p4);
};

}
