/*----------------------------------------------------------------------
#                                                                      #
# Software Name : REVOCAP_PrePost version 1.6                          #
# Class Name : AFTetraMesher                                           #
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
#include "MeshDB/kmbFace.h"

namespace kmb{

class Point3DContainer;
class ElementContainer;
class ElementEvaluator;
class MeshData;
class NodeNeighborInfo;

class PatchModifier
{
protected:
	kmb::Point3DContainer* points;
	kmb::ElementContainer* patch;
	kmb::ElementEvaluator* evaluator;
	kmb::NodeNeighborInfo* neighborInfo;
public:
	PatchModifier(kmb::Point3DContainer* points,kmb::ElementContainer* patch);
	PatchModifier(kmb::MeshData* mesh,kmb::bodyIdType bodyId);
	virtual ~PatchModifier(void);
	// ���������낦��B������ς����v�f�̌���Ԃ��B
	int uniformOrientation(kmb::elementIdType elementId=kmb::Element::nullElementId);
	// �אڂ���O�p�`�̕ӂ���������
	// [0,1,3] + [1,2,3] => [0,1,2] + [2,3,0]
	bool triangleEdgeSwap(kmb::elementIdType tri0, kmb::elementIdType tri1 );
	// �אڂ���l�p�`�̕ӂ���������
	// [0,1,2,3] + [0,3,4,5] => [1,2,3,4] + [0,1,4,5]  (orientation=true)
	//                       => [0,1,2,5] + [2,3,4,5]  (orientation=false)
	bool quadEdgeSwap(kmb::elementIdType quad0, kmb::elementIdType quad1, bool orientation=true);
	// �אڂ���l�p�`�ɂP�_�������ĂR�̎l�p�`�ɂ���
	// �ł���p�^�[���͂Q�ʂ�
	// orientation true => quad0 �� 0 �Ԗڂ���ӂ��o��
	kmb::elementIdType quadTrisect(kmb::elementIdType quad0, kmb::elementIdType quad1, bool orientation=true );
	// �אڂ��Ă���v�f�ԍ���Ԃ�
	int getNeighborElements(kmb::elementIdType elementId,kmb::elementIdType* neighbors) const;
	kmb::elementIdType getNeighborElement(kmb::Face f) const;
	// �ӂ̏k��
	bool edgeContract(kmb::nodeIdType node0, kmb::nodeIdType node1);
	// ���_�̂Ȃ��p�x��Ԃ�
	double getAngleByIndex(kmb::elementIdType elementId,int index) const;
	// TODO : ���Ȏl�p�`���������Ƃ��ɁA�C������
};

}
