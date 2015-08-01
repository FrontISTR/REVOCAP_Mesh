/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : PatchOperation                                          #
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

#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbPolygon.h"

namespace kmb{

class MeshData;
class MeshDB;
class Point3DContainer;
class ElementContainer;
class Polygon;

class EdgeStitcher
{
private:
	kmb::MeshData* mesh;
	kmb::Point3DContainer* points;
	kmb::Polygon forward;			// �����������̑��p�`
	kmb::nodeIdType f0;				// forward ���p�`�̒��ڐߓ_
	kmb::nodeIdType fend;			// forward ���p�`�̏I���ߓ_
	kmb::Polygon backward;			// �t���������̑��p�`
	kmb::nodeIdType b0;				// backward ���p�`�̒��ڐߓ_
	kmb::nodeIdType bend;			// backward ���p�`�̏I���ߓ_
	kmb::Vector3D normal;			// ����p�̖@���x�N�g��
	bool normalCheck;				// �@���`�F�b�N�����邩�ǂ���
	void setInitNormal(void);		// �����@���x�N�g�����v�Z����

public:
	EdgeStitcher(void);
	virtual ~EdgeStitcher(void);
	
	// for ruby interface
	void setMesh(kmb::MeshData* mesh);
	// FaceGroup �� Edge ��^����
	// �e Body ��K���^���Ă����Ȃ���΂Ȃ�Ȃ�
	bool setEdges(const char* forward,const char* backward);
	bool setEdges(kmb::bodyIdType forward,kmb::bodyIdType backward);
	bool setEdgesWithinDistance(kmb::bodyIdType forward,kmb::bodyIdType backward,double distance);
	bool setEdgesWithInitNodes(kmb::bodyIdType forward,kmb::nodeIdType forwardInit,kmb::bodyIdType backward,kmb::nodeIdType backwardInit);
	kmb::bodyIdType stitchEdgesByTriangles(void);
	kmb::bodyIdType stitchEdgesByQuads(void);
	// ���E�̗v�f�̌����`�F�b�N���Ȃ���D�����킹��
	// fCount : forward �����E�Ɏ��悤�� patch �̗v�f��
	// bCount : backward �����E�Ɏ��悤�� patch �̗v�f��
	kmb::bodyIdType stitchByQuadsWithNodeCount(int fCount,int bCount);

	void setNormalCheck(bool check);
	bool getNormalCheck(void) const;

	bool stitchByTriangle( kmb::ElementContainer* result );
	bool stitchByQuad( kmb::ElementContainer* result );
	// �ŏ��̒��ڐߓ_��ݒ肷��B
	// nullNodeId ����ꂽ�ꍇ�́A��ԋ߂��Ƃ���œK���ɐݒ肷��B
	void setInitialNode( kmb::nodeIdType forwardInit=kmb::nullNodeId, kmb::nodeIdType backwardInit=kmb::nullNodeId );
};

}
