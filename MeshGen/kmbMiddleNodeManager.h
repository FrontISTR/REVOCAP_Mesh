/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : MiddleNodeManager                                       #
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
// �P���v�f���Q���v�f�ɂ���ꍇ�ȂǁA�ӂ̗��[�̓_��^���āA���̒��ԓ_��Ԃ�
//
// �l�p�`�̎l�_�̏��Ԃ�����Ă����S�̓_�𐳂����Ԃ��悤�ȃR���e�i�̐݌v
//
// NodePair �ɂ͑Ίp���̂����A�ŏ��̐ߓ_�ԍ�����̍Œ��Ίp����o�^����
// �Ίp�������݂��Ȃ����́i�O�p�`�Ȃǁj�͍ŏ��̐ߓ_�ԍ��̑Εӂ̒��Ԑߓ_�̃y�A��o�^����
//
// middlePoints �� map �� Face �������ƁA���Ԑߓ_�ɑ΂��� Face ��
// ��ӂł͂Ȃ��̂ł悭�Ȃ��̂ł��̂悤�Ȏ����Ƃ���
//
#pragma once

#include "MeshDB/kmbTypes.h"
#include "MeshDB/kmbElement.h"
#include <map>

namespace kmb{

class Point3DContainer;
class ElementBase;
class MeshSmoother;

class MiddleNodeManager
{
protected:
	struct originalPair{
		kmb::nodeIdType n0;
		kmb::nodeIdType n1;
		// �v�f�̒��S�E�ʂ��琶�������Ƃ��B����ȊO�� kmb::Element::nullElementId
		kmb::elementIdType elementId;
	};
	// ��ꐬ���̕����������Ƃ���
	typedef std::pair< kmb::nodeIdType, kmb::nodeIdType > NodePair;
	Point3DContainer* points;
	// �e�ߓ_�̑g => ���Ԑߓ_
	std::map< NodePair, kmb::nodeIdType > middlePoints;
	// ���Ԑߓ_ => �e�ߓ_�̏��
	std::map< kmb::nodeIdType, kmb::MiddleNodeManager::originalPair > originalPoints;
protected:
	// �����̎Q�ƃe�[�u���ɒǉ�����
	void appendMiddleNode( kmb::nodeIdType middle, kmb::nodeIdType org0, kmb::nodeIdType org1, kmb::elementIdType elemId=kmb::Element::nullElementId);
	// ���������_�𐶐����Ă��� nodeId ��Ԃ�
	virtual kmb::nodeIdType createMiddleNode(kmb::nodeIdType n0, kmb::nodeIdType n1);
	virtual kmb::nodeIdType createMiddleNode3(kmb::nodeIdType n0, kmb::nodeIdType n1, kmb::nodeIdType n2);
	virtual kmb::nodeIdType createMiddleNode4(kmb::nodeIdType n0, kmb::nodeIdType n1,kmb::nodeIdType n2, kmb::nodeIdType n3);
	virtual kmb::nodeIdType createMiddleNode6
		(kmb::nodeIdType n0, kmb::nodeIdType n1,kmb::nodeIdType n2,
		 kmb::nodeIdType n3, kmb::nodeIdType n4,kmb::nodeIdType n5);
	// 0.125 * (n0 + n1 + n2 + n3 + n4 + n5 + n6 + n7)
	virtual kmb::nodeIdType createMiddleNode8
		(kmb::nodeIdType n0, kmb::nodeIdType n1,kmb::nodeIdType n2, kmb::nodeIdType n3,
		 kmb::nodeIdType n4, kmb::nodeIdType n5,kmb::nodeIdType n6, kmb::nodeIdType n7);
	// 0.375 * n0 + 0.75 * n1 - 0.125 * n2
	virtual kmb::nodeIdType createMiddleNode3s(kmb::nodeIdType n0, kmb::nodeIdType n1, kmb::nodeIdType n2);
	// 0.5 * n0 + 0.5 * n1 - 0.125 * n2 - 0.125 * n3 + 0.25 * n4
	virtual kmb::nodeIdType createMiddleNode5(kmb::nodeIdType n0, kmb::nodeIdType n1, kmb::nodeIdType n2, kmb::nodeIdType n3, kmb::nodeIdType n4);
	// -0.25 * (n0 + n1 + n2 + n3) + 0.5 * (n4 + n5 + n6 + n7)
	virtual kmb::nodeIdType createMiddleNode8s
		(kmb::nodeIdType n0, kmb::nodeIdType n1,kmb::nodeIdType n2, kmb::nodeIdType n3,
		 kmb::nodeIdType n4, kmb::nodeIdType n5,kmb::nodeIdType n6, kmb::nodeIdType n7);
public:
	MiddleNodeManager(void);
	virtual ~MiddleNodeManager(void);
	void clear(void);
	// �R���e�i�̐ݒ�
	void setNodeContainer(Point3DContainer* points);
	size_t getMiddleNodeCount(void) const;
	// a �� b �̒��Ԑߓ_�� nodeId ��Ԃ�
	// �o�^����Ă��Ȃ�������_�𐶐����AnodeContainer �ɒǉ����Ă���Ԃ�
	// a �� b ����Ȃ�v�f(SEGMENT)�̏ꍇ�� elemId ��^����
	kmb::nodeIdType getDividedNode(kmb::nodeIdType a,kmb::nodeIdType b,kmb::elementIdType elemId=kmb::Element::nullElementId);
	// �v�f�̒��Ԑߓ_��^����̂�
	// SEGMENT QUAD HEXAHEDRON �̂Ƃ�
	// TRIANGLE TETRAHEDRON WEDGE �̂Ƃ��FmiddlePoints �ɂ͑Εӂ̒��Ԑߓ_��^����
	kmb::nodeIdType getCenterNode(const kmb::ElementBase &elem,kmb::elementIdType elementId);
	kmb::nodeIdType getCenterNode(const kmb::ElementBase &elem,int faceIndex,kmb::elementIdType elementId);
	// ���ڐߓ_�ԍ��ŗ^����ꍇ
	kmb::nodeIdType getCenterNode4(kmb::nodeIdType n0,kmb::nodeIdType n1,kmb::nodeIdType n2,kmb::nodeIdType n3,kmb::elementIdType elementId);
	kmb::nodeIdType getCenterNode8(kmb::nodeIdType n0,kmb::nodeIdType n1,kmb::nodeIdType n2,kmb::nodeIdType n3,kmb::nodeIdType n4,kmb::nodeIdType n5,kmb::nodeIdType n6,kmb::nodeIdType n7,kmb::elementIdType elementId);
	// 2���v�f�̎��Ɍ`��֐����g���Ē��Ԑߓ_��^����
	kmb::nodeIdType getCenterNode2(const kmb::ElementBase &elem,kmb::elementIdType elementId);
	kmb::nodeIdType getCenterNode2(const kmb::ElementBase &elem,int faceIndex,kmb::elementIdType elementId);
	// a �� b �̒��Ԑߓ_�� nodeId ��Ԃ�
	// �o�^����Ă��Ȃ������� nullNodeId ��Ԃ�
	kmb::nodeIdType isDivided(kmb::nodeIdType a,kmb::nodeIdType b) const;
	// a �� b �̒��_�� middle �ł��邱�Ƃ��o���Ă���
	void setDividedNode(kmb::nodeIdType middle, kmb::nodeIdType a,kmb::nodeIdType b);
	// middle point ���猳�̓_����������
	kmb::elementIdType getOriginalNode(kmb::nodeIdType centerId,kmb::nodeIdType &a,kmb::nodeIdType &b) const;

	// a �� b �̒��Ԑߓ_�� nodeId ��Ԃ�
	// a, b, c ��3�_�����̏��ԂŒʂ�2���Ȑ���ɒ��Ԑߓ_�����
	// �o�^����Ă��Ȃ�������_�𐶐����AnodeContainer �ɒǉ����Ă���Ԃ�
	// a �� b ����Ȃ�v�f(SEGMENT)�̏ꍇ�� elemId ��^����
	kmb::nodeIdType getDividedNode3(kmb::nodeIdType a,kmb::nodeIdType b,kmb::nodeIdType c,kmb::elementIdType elemId=kmb::Element::nullElementId);

	// n0 �� n1 �̒��Ԑߓ_�� nodeId ��Ԃ�
	// �O�p�`2���� 4,5,2,1,3 �̏�
	kmb::nodeIdType getDividedNode5(kmb::nodeIdType n0,kmb::nodeIdType n1,kmb::nodeIdType n2,kmb::nodeIdType n3,kmb::nodeIdType n4,kmb::elementIdType elemId=kmb::Element::nullElementId);

	// �l�p���Q���̓����̒��Ԑߓ_���`��֐������邽�߂̓��ʃ��\�b�h
	// index = 5, 6, 7, 8 �ƒ�̒��S c �̊Ԃ̒��_�𐶐�����
	kmb::nodeIdType getDividedNodePyrmid2c(int index,kmb::nodeIdType center,const kmb::ElementBase &elem,kmb::elementIdType elemId=kmb::Element::nullElementId);

	// ���Ԑߓ_�̈ʒu�𒲐�����
	// MeshSmoother ��^���āA�ߓ_�𓮂���
	// �������Ώۂ͒��Ԑߓ_�ł̂�
	// �߂�l�͓��������ߓ_�̌�
	// smoother �ɂ��炩���� mesh �� body ��o�^���Ă�������
	virtual int smoothingMiddleNodes(kmb::MeshSmoother* smoother);
};

}
