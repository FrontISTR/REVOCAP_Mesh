/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : TriangleBucketArea                                      #
#                                                                      #
#                                Written by                            #
#                                           K. Tokunaga 2012/03/23     #
#                                                                      #
#      Contact Address: IIS, The University of Tokyo CISS              #
#                                                                      #
#      "Large Scale Assembly, Structural Correspondence,               #
#                                     Multi Dynamics Simulator"        #
#                                                                      #
----------------------------------------------------------------------*/
//
// ��Ԃ��i�q��ɕ��������̈悲�ƂɎO�p�`�Ɗ����Ă���ʐς�ۑ�����
// �i�[����Ă���ʐς����ׂđ����΁A���̎O�p�`�̖ʐςɂȂ�i���l�덷�͈̔͂Łj
//
#pragma once

#include "MeshDB/kmbTypes.h"
#include "Geometry/kmbBucket.h"
#include "Geometry/kmbGeometry3D.h"

#include <vector>

namespace kmb{

class ElementContainer;
class Point3DContainer;
class Matrix4x4;
class DataBindings;

class TriangleBucketArea : public Bucket< std::pair<kmb::elementIdType,double> >
{
private:
	const kmb::Point3DContainer* points;
	const kmb::ElementContainer* elements;
	const kmb::Matrix4x4* coordMatrix;
	kmb::DataBindings* elemArea;
public:
	TriangleBucketArea(void);
	TriangleBucketArea(const kmb::BoxRegion &box,int xnum,int ynum,int znum);
	virtual ~TriangleBucketArea(void);

	void setContainer(const kmb::Point3DContainer* points,const kmb::ElementContainer* elements,const kmb::Matrix4x4* coordMatrix=NULL);

	int append(kmb::elementIdType elementId);
	int appendAll(void);

	bool getNearest(double x,double y,double z,double &dist,kmb::elementIdType &tri) const;
	double getArea(kmb::elementIdType elemId) const;
protected:
	// (i,j,k) �� bucket �̒��ł̍ŏ��l
	// �����͂Q��ŕԂ�
	bool getNearestInBucket(const kmb::Point3D& pt,int i,int j,int k,double &dist,kmb::elementIdType &tri) const;
};

}
