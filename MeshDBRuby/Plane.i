/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Ruby Class Name : FramedPlane                                        #
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
/**--------------------------------------------------------------------
=begin
= RevocapMesh::FramedPlane

=end
---------------------------------------------------------------*/
#include "Geometry/kmbPlane.h"
#include "Geometry/kmbFramedPlane.h"

namespace kmb{

class Plane
{
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
	// 法線の変更
	bool setNormal(double a,double b,double c);
	void setOrigin(double x,double y,double z);
	// 空集合かもしれないからポインタで返す
	Point3D* createIntersectPoint(const Point3D &p0,const Point3D &p1) const;
	// p0 と p1 を結ぶ直線との交点
	// p0 を t=0、p1 を t=1 としてパラメータを与えたときの t も計算する
	bool getIntersection(const Point3D &p0,const Point3D &p1, Point3D &pt, double &t) const;
	// get foot of perpendicular
	Point3D projectOnPlane(const Point3D& p) const;
};

class FramedPlane : public Plane
{
public:
	FramedPlane(const double a0,const double b0,const double c0,const double d0);
	FramedPlane(const Point3D &o,const Vector3D &v0,const Vector3D &v1);
	virtual ~FramedPlane(void);
	// origin を原点とするような UV 座標を返す
	Point2D transformTo2D(const Point3D &point) const;
	Point3D transformTo3D(const Point2D &point) const;
	Point3D transformTo3D(double u,double v) const;
	// UV 座標の基底ベクトルを回転する
	void rotateUVBase(const double angle);
	// 原点の移動
	void setOrigin(const Point3D& point);
	void setOrigin(double x,double y,double z);
	// origin = origin + s * normal
	void slideOrigin(double s);
	// 法線の変更
	bool setNormal(double a,double b,double c);
	// ラジアン
	void setNormalPolar(double phi,double theta);
/**--------------------------------------------------------------------
=begin
--- getOrigin()
	原点の座標を返す。
=end
---------------------------------------------------------------------*/
%extend{
	VALUE getOrigin(void)
	{
		VALUE ary = rb_ary_new();
		kmb::Point3D ori;
		self->getOrigin( ori );
		rb_ary_store(ary,0,rb_float_new(ori.x()));
		rb_ary_store(ary,1,rb_float_new(ori.y()));
		rb_ary_store(ary,2,rb_float_new(ori.z()));
		return ary;
	}
}
/**--------------------------------------------------------------------
=begin
--- getNormalPolar()
	法線ベクトルの球面極座標の値 (phi,theta) を返す。ただし、
	-PI < phi <= PI および 0 <= theta <= PI
=end
---------------------------------------------------------------------*/
%extend{
	VALUE getNormalPolar(void)
	{
		VALUE ary = rb_ary_new();
		double phi=0.0,theta=0.0;
		self->getNormalPolar(phi,theta);
		rb_ary_store(ary,0,rb_float_new(phi));
		rb_ary_store(ary,1,rb_float_new(theta));
		return ary;
	}
}
/**--------------------------------------------------------------------
=begin
--- getEquation()
	この平面が表す方程式 ax+by+cz+d=0 を Array [a,b,c,d] で返す。
	[a,b,c] は正規化されているものとする。
=end
---------------------------------------------------------------------*/
%extend{
	VALUE getEquation(void)
	{
		VALUE ary = rb_ary_new();
		kmb::Vector3D normal = self->getNormal();
		rb_ary_store(ary,0,rb_float_new(normal.x()));
		rb_ary_store(ary,1,rb_float_new(normal.y()));
		rb_ary_store(ary,2,rb_float_new(normal.z()));
		rb_ary_store(ary,3,rb_float_new(self->getConstant()));
		return ary;
	}
}
};

}
