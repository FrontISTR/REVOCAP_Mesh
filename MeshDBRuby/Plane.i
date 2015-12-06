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

class FramedPlane : public Plane
{
public:
	FramedPlane(const double a0,const double b0,const double c0,const double d0);
	FramedPlane(const Point3D &o,const Vector3D &v0,const Vector3D &v1);
	virtual ~FramedPlane(void);
	// origin �����_�Ƃ���悤�� UV ���W��Ԃ�
	Point2D transformTo2D(const Point3D &point) const;
	Point3D transformTo3D(const Point2D &point) const;
	Point3D transformTo3D(double u,double v) const;
	// UV ���W�̊��x�N�g������]����
	void rotateUVBase(const double angle);
	// ���_�̈ړ�
	void setOrigin(const Point3D& point);
	void setOrigin(double x,double y,double z);
	// origin = origin + s * normal
	void slideOrigin(double s);
	// �@���̕ύX
	bool setNormal(double a,double b,double c);
	// ���W�A��
	void setNormalPolar(double phi,double theta);
/**--------------------------------------------------------------------
=begin
--- getOrigin()
	���_�̍��W��Ԃ��B
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
	�@���x�N�g���̋��ʋɍ��W�̒l (phi,theta) ��Ԃ��B�������A
	-PI < phi <= PI ����� 0 <= theta <= PI
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
	���̕��ʂ��\�������� ax+by+cz+d=0 �� Array [a,b,c,d] �ŕԂ��B
	[a,b,c] �͐��K������Ă�����̂Ƃ���B
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
