/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : TetraItem                                               #
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
// list �I�Ȏl�ʑ̂̃R���e�i
// �אڏ����ێ�
// neighborFace 
// -2 : �Q�Ƃ��Ȃ�
// -1 : �אڂ��� tetra ���Ȃ�
// 0, 1, 2, 3 : �א� tetra �̑��葤�̖ʔԍ�
//
#pragma once

#include "MeshDB/kmbTetrahedron.h"
#include "MeshGen/kmbListItem.h"

namespace kmb{

class Point3DContainer;

class TetraN : public Tetrahedron
{
protected:
	TetraN** neighbors;
	// �ߖT���
	// �������ߖ�̂��߂̓����\�� int[4](-2�`3)
	// -1 : �אڂ��� tetra ���Ȃ�
	// -2 : �Q�Ƃ��Ȃ�
	unsigned int face;
public:
	TetraN(nodeIdType n0,nodeIdType n1,nodeIdType n2,nodeIdType n3);
	virtual ~TetraN(void);
	// [this,index] <=> [nei,f] ���ڂ���
	void setNeighbor(int index, kmb::TetraN* nei,int f);
	// 0 1 2 3 �� getNeighbor �œ�����אڂ��� tetra ����݂��������g���ڂ���ʔԍ�
	// [this,i] �� [getNeighbor(i),getNeighborFace(i)] ���אڂ���
	kmb::TetraN* getNeighbor(int index) const;
	int getNeighborFace(int index) const;
	// nei �����Ԗڂ̖ʂɗאڂ��Ă��邩
	int getNeighborIndex(kmb::TetraN* nei) const;
};

class TetraItem : public Tetrahedron
{
protected:
	kmb::TetraItem* previousTetra;
	kmb::TetraItem* nextTetra;
	kmb::TetraItem* neighbors[4];
	// �ߖT���
	// �������ߖ�̂��߂̓����\�� int[4](-2�`3)
	// -1 : �אڂ��� tetra ���Ȃ�
	// -2 : �Q�Ƃ��Ȃ�
	unsigned int face;
public:
	TetraItem(nodeIdType n0,nodeIdType n1,nodeIdType n2,nodeIdType n3);
	virtual ~TetraItem(void);
	// list item
	kmb::TetraItem* getPrevious(void) const { return previousTetra; }
	void setPrevious(kmb::TetraItem* tetra){ previousTetra = tetra; }
	kmb::TetraItem* getNext(void) const { return nextTetra; }
	void setNext(kmb::TetraItem* tetra){ nextTetra = tetra; }

	// ���X�g����O��
	// this->nextTetra->previousTetra �� this ���� this->previousTetra �ɕς���
	// this->previousTetra->nextTetra �� this ���� this->nextTetra �ɕς���
	// ������ previousTetra �� nextTetra �� NULL �ɂ���
	void detach(void);
	void detach(kmb::TetraItem* &first,kmb::TetraItem* &last);
	// ���X�g�ɒǉ�
	void attach(kmb::TetraItem* last);
	void attach(kmb::TetraItem* &first,kmb::TetraItem* &last);

	// [this,index] <=> [nei,f] ���ڂ���
	void setNeighbor(int index, kmb::TetraItem* nei,int f);
	// 0 1 2 3 �� getNeighbor �œ�����אڂ��� tetra ����݂��������g���ڂ���ʔԍ�
	// [this,i] �� [getNeighbor(i),getNeighborFace(i)] ���אڂ���
	kmb::TetraItem* getNeighbor(int index) const;
	int getNeighborFace(int index) const;
	// nei �����Ԗڂ̖ʂɗאڂ��Ă��邩
	int getNeighborIndex(kmb::TetraItem* nei) const;

	// tetra ����n�܂郊�X�g�ɂ��āA�ߖT�֌W�𐶐�����
	static void generateNeighborRelations( kmb::TetraItem* tetra );

	// t0 �� i0 �Ԗڂ̖ʂ� t1 �� i1 �Ԗڂ̖ʂ��ڂ��Ă���Ƃ�
	static void setAdjacentPair(kmb::TetraItem* t0,int i0,kmb::TetraItem* t1,int i1);
	// first ����n�܂郊�X�g���폜
	// �폜�����炻��ɗאڂ��Ă��� tetra �� neighborface �� -2 �ɂ���
	static int deleteItems( kmb::TetraItem* first );

	// for debug
	static int debugItem( kmb::TetraItem* item, const char* tag=NULL  );
	static int debugItems( kmb::TetraItem* first, const char* tag=NULL );
	static bool checkVolume( kmb::TetraItem* tetra, kmb::Point3DContainer &points );
	static bool checkVolumes( kmb::TetraItem* tetra, kmb::Point3DContainer &points, const char* tag=NULL );
	static bool checkNeighbor( kmb::TetraItem* tetra );
	static bool checkNeighbors( kmb::TetraItem* tetra, const char* tag=NULL );
};

}
