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
#pragma once

#include "Geometry/kmbGeometry3D.h"
#include "Geometry/kmbRegion.h"
#include "Geometry/kmbSphere.h"

namespace kmb{

/// ���W���ɕ��s�Ȓ�����
class BoxRegion : public Region
{
protected:
	// ������臒l
	static double thres;
protected:
	Point3D maxPoint;
	Point3D minPoint;
public:
	BoxRegion(void);
	BoxRegion(const Point3D &l,const Point3D &u);
	BoxRegion(double x0,double y0,double z0,double x1,double y1,double z1);
	BoxRegion(const BoxRegion &other);
	virtual ~BoxRegion(void);
	BoxRegion& operator=(const BoxRegion &other);

	void setMinMax(double x0,double y0,double z0,double x1,double y1,double z1);
	void setMinMax(kmb::Point3D& minPoint,kmb::Point3D& maxPoint);
	double minX(void) const;
	double minY(void) const;
	double minZ(void) const;
	double maxX(void) const;
	double maxY(void) const;
	double maxZ(void) const;
	void minX(double x);
	void minY(double y);
	void minZ(double z);
	void maxX(double x);
	void maxY(double y);
	void maxZ(double z);
	double centerX(void) const;
	double centerY(void) const;
	double centerZ(void) const;
	double rangeX(void) const;
	double rangeY(void) const;
	double rangeZ(void) const;
	// rangeX rangeY rangeZ �̑傫������Ԃ�
	double range(void) const;
	double diameter(void) const;
	double diameterSq(void) const;
	const Point3D& getMin(void) const;
	const Point3D& getMax(void) const;
	void getCenter(kmb::Point3D& center) const;
	double getVolume(void) const;

	virtual kmb::Region::locationType intersect(const kmb::Point3D &point) const;
	virtual kmb::Region::locationType intersect(const double x,const double y,const double z) const;
	virtual double distanceSq(const kmb::Point3D &point) const;
	virtual double distanceSq(const double x,const double y,const double z) const;
	// Box ���m�̔���֐�
	bool intersect(const kmb::BoxRegion& box) const;
	double intersectVolume(const BoxRegion& box) const;
	double distanceSq(const kmb::BoxRegion& box) const;
	// max min �̗������g�傷��
	void expand(double x,double y,double z);
	void expand(double ratio);
	// ���s�ړ�
	void translate(double x,double y,double z);
	// center ��ς����� range ��ς���
	void setRange(double rangeX,double rangeY,double rangeZ);
	// diameter �� ratio �{�ɂȂ�悤�� x y z �𓯂����������L����
	void expandDiameter(double ratio);
	// �����Əd�Ȃ��Ă��镔��
	// �d�Ȃ��Ă��Ȃ���΁Amint_t = max_t = 0.0 ��Ԃ�
	void crossOnLine(const kmb::Point3D& origin, const kmb::Vector3D& dir, double &min_t, double &max_t) const;
	// �O�p�`�Ƃ̔���
	// ab bc ca ��6�ʂƌ������邩�ǂ����̔���
	// ���̎����ł� intersectArea > 0.0 �Ƃ�����������
	bool intersect(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const;
	// intersectArea ���� Area ������������
	bool intersect3(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const;
	// �O�p�`�Ƃ̔���i�ʐς��j
	// �@���x�N�g�����ق����ꍇ�������Ŗʐς��v�Z���Ă���@����V���Ɍv�Z����ق�������
	// ���R�FVector3D �^��߂�l�ɂ���I�[�o�[�w�b�h���傫��
	double intersectArea(const kmb::Point3D &a,const kmb::Point3D &b,const kmb::Point3D &c) const;
private:
	// ����������
	// ���̏��ɏォ��Ă΂��
	double intersectArea_minx(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	double intersectArea_maxx(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	double intersectArea_miny(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	double intersectArea_maxy(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	double intersectArea_minz(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	double intersectArea_maxz(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;

	bool intersect_minx(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	bool intersect_maxx(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	bool intersect_miny(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	bool intersect_maxy(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	bool intersect_minz(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
	bool intersect_maxz(const kmb::Point3D &p0,const kmb::Point3D &p1,const kmb::Point3D &p2) const;
};

}
