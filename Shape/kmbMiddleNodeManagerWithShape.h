/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : FittingToSurface                                        #
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
#pragma once

#include "MeshGen/kmbMiddleNodeManager.h"
#include <vector>

namespace kmb{

template <typename T> class Vector2WithIntBindings;
template <typename T> class NodeMapperBindings;
class Surface3D;
class Point3D;
class MeshSmoother;

class MiddleNodeManagerWithShape : public kmb::MiddleNodeManager
{
protected:
	kmb::Vector2WithIntBindings<kmb::nodeIdType>* mappingToSurface;
	const std::vector< kmb::Surface3D* >* surfaces;
	// (u,v) ���W�̒��_�ł͂Ȃ��A���[�N���b�h���W�̒��_�̍ŋߓ_�����߂�
	bool nearestFlag;
	int iterMax;
	double epsilon;
public:
	MiddleNodeManagerWithShape(void);
	virtual ~MiddleNodeManagerWithShape(void);
	void setMappingData(kmb::Vector2WithIntBindings<kmb::nodeIdType>* mapping);
	void setSurfaces(const std::vector< kmb::Surface3D* >* surfaces);
	void setNearestFlag(bool b);
	void setiterMax(int m);
	void setEpsilon(double e);
	// Refiner �p�� Global �ߓ_�ԍ��ƋǏ��ߓ_�ԍ���ϊ�����Ƃ��Ɏg��
	// �ߓ_�ԍ��̒u�������A�߂�l�͒u����������
	size_t replaceNodeIds( const std::map<kmb::nodeIdType,kmb::nodeIdType> &nodeMapper );
	// �ߓ_�ԍ���^���āA�K���Ώۂ̋Ȗʂ̌��ƁA���̋Ȗʏ�̍��W��Ԃ�
	// mappingToSurface �ɋȖʂ��B��^�����Ă���΁A���̋Ȗʍ��W (u,v) �����ԍ��W�����߂�
	// ��ȏ�^�����Ă���΂����̕���
	// �߂�l�͋Ȗʂ̌� 0 �̎��͍��W�͕ω����Ȃ�
	int getPointOnSurface(kmb::nodeIdType nodeId,kmb::Point3D &pt) const;
protected:
	// surface �̏�ɏ��\���̂�����̂��Ē�`
	virtual kmb::nodeIdType createMiddleNode(kmb::nodeIdType n0, kmb::nodeIdType n1);
	virtual kmb::nodeIdType createMiddleNode4(kmb::nodeIdType n0, kmb::nodeIdType n1,kmb::nodeIdType n2, kmb::nodeIdType n3);
};

}
